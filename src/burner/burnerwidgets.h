#ifndef BURNERWIDGETS_H
#define BURNERWIDGETS_H

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

struct BurnerProgressBarsWidgets
{
    QProgressBar* progress_bar_burn = nullptr;
    QProgressBar* progress_bar_ring_buffer_unit = nullptr;
    QProgressBar* progress_bar_unit_buffer_unit = nullptr;
};

#endif // BURNERWIDGETS_H
