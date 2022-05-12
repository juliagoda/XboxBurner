#ifndef BURNERSTAGE_H
#define BURNERSTAGE_H

#include "burnerprogressbarssetup.h"

#include <QString>
#include <QProcess>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QProgressBar>
#include <QFutureWatcher>
#include <QStatusBar>

struct BurnerWidgets
{
    QComboBox* combo_box_dvd_format = nullptr;
    QCheckBox* check_box_dao_mode = nullptr;
    QCheckBox* check_box_dvd_compat = nullptr;
    QCheckBox* check_box_dry_run = nullptr;
    QComboBox* combo_box_write_speed = nullptr;
    QLineEdit* lineedit_burner_path = nullptr;
    QLineEdit* lineedit_image_path = nullptr;

    QPlainTextEdit* plain_text_edit_with_logs = nullptr;
    QToolBar* toolbar = nullptr;
    QProgressBar* progress_bar_burn = nullptr;
    QProgressBar* progress_bar_ring_buffer_unit = nullptr;
    QProgressBar* progress_bar_unit_buffer_unit = nullptr;

    QFutureWatcher<bool>* backup_future_watcher = nullptr;
    QCheckBox* check_box_backup_creation = nullptr;
    QStatusBar* status_bar = nullptr;
};

class BurnerStage : public QObject
{
    Q_OBJECT

public:
    BurnerStage(const BurnerWidgets& new_burner_widgets) :
        burner_widgets(new_burner_widgets),
        burner_progress_bars_setup(
            QSharedPointer<BurnerProgressBarsSetup>(
                new BurnerProgressBarsSetup({new_burner_widgets.progress_bar_burn,
                                             new_burner_widgets.progress_bar_ring_buffer_unit,
                                             new_burner_widgets.progress_bar_unit_buffer_unit}))),
        next_stage{nullptr},
        burn_process{nullptr},
        burning{false}
    {

}

    QSharedPointer<BurnerStage> next(QSharedPointer<BurnerStage> next_burner_stage)
    {
        next_stage = next_burner_stage;
        return next_stage;
    }

    virtual bool handle()
    {
        if (next_stage)
            return next_stage->handle();

        return true;
    }

protected:
    BurnerWidgets burner_widgets;
    QSharedPointer<BurnerProgressBarsSetup> burner_progress_bars_setup;
    QSharedPointer<BurnerStage> next_stage;
    QProcess* burn_process;
    bool burning;
};

#endif // BURNERSTAGE_H
