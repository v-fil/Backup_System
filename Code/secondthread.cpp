#include "Headers/secondthread.h"

secondThread::secondThread(QDir _sDir, QDir _dDir, QFileInfoList _diffList, QFileInfoList _diffList1, quint64 _sumSize, mainWnd *_wnd)
{
    sDir = _sDir;
    dDir = _dDir;
    diffList = _diffList;
    diffList1 = _diffList1;
    sumSize = _sumSize;
    wnd = _wnd;
}

void secondThread::run()
{
    curSize = 0;
    emit wnd->newLogLineSignal(time->currentTime().toString("\nh:m:s") + ": Копіюю файли з папки 1");
    foreach(QFileInfo diffInfo, diffList)
    {
        QString backupPath = diffInfo.filePath().replace(sDir.absolutePath(), dDir.absolutePath());
        if (diffInfo.isFile())
        {
            emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Копіюю файл " + diffInfo.absoluteFilePath());
            QFile::copy(diffInfo.absoluteFilePath(), backupPath);
            curSize += diffInfo.size();
            emit wnd->updateProgressBarSignal(int((curSize * 100) / sumSize));
        }
        if (diffInfo.isDir())
        {
            emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Створюю папку " + diffInfo.absoluteFilePath());
            dDir.mkdir(backupPath);
        }
        this->thread()->msleep(5);
    }

    emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Копіюю файли з папки 2");
    foreach(QFileInfo diffInfo, diffList1)
    {
        QString backupPath = diffInfo.filePath().replace(dDir.absolutePath(), sDir.absolutePath());
        if (diffInfo.isFile())
        {
            emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Копіюю файл " + diffInfo.absoluteFilePath());
            QFile::copy(diffInfo.absoluteFilePath(), backupPath);
            curSize += diffInfo.size();
            emit wnd->updateProgressBarSignal(int((curSize * 100) / sumSize));
        }
        if (diffInfo.isDir())
        {
            emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Створюю папку " + diffInfo.absoluteFilePath());
            dDir.mkdir(backupPath);
        }
        this->thread()->msleep(5);
    }
    emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Копіювання завершено\n");
    if (wnd->closeBool){
        emit wnd->newLogLineSignal(time->currentTime().toString("h:m:s") + ": Завершення програми через 10 секунд");
        this->thread()->sleep(10);
        QCoreApplication::exit(0);
    }
}
