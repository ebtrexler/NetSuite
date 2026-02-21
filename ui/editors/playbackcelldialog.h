#ifndef PLAYBACKCELLDIALOG_H
#define PLAYBACKCELLDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "RT_PlaybackCell.h"
#include "playbackwaveformdialog.h"

class PlaybackCellDialog : public QDialog
{
    Q_OBJECT
public:
    PlaybackCellDialog(TPlaybackCell *cell, QWidget *parent = nullptr)
        : QDialog(parent), m_cell(cell)
    {
        setWindowTitle(QString("Playback Cell: %1").arg(QString::fromStdWString(cell->Name())));
        auto *layout = new QVBoxLayout(this);

        wfWidget = new PlaybackWaveformWidget(&cell->Waveform(), "mV", this);
        layout->addWidget(wfWidget);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &PlaybackCellDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

private slots:
    void tryAccept() {
        if (!wfWidget->applyToWaveform()) return;
        accept();
    }

private:
    TPlaybackCell *m_cell;
    PlaybackWaveformWidget *wfWidget;
};

#endif
