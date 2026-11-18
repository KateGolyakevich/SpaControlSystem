#ifndef SPAPACKAGEBUILDERDIALOG_H
#define SPAPACKAGEBUILDERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QLabel>
#include <vector>
#include "core/SpaSystem.h"

class SpaPackageBuilderDialog : public QDialog {
    Q_OBJECT

private:
    SpaSystem* spa_system;
    std::vector<int> selected_service_ids;

    QLineEdit* edit_title;
    QDoubleSpinBox* spin_discount;
    QListWidget* list_available;
    QListWidget* list_selected;
    QLabel* label_summary;

    void setup_ui();
    void populate_available_services();
    void update_summary();

public:
    explicit SpaPackageBuilderDialog(SpaSystem* system, QWidget* parent = nullptr);
    ~SpaPackageBuilderDialog() override = default;

private slots:
    void on_btn_add_service_clicked();
    void on_btn_remove_service_clicked();
    void on_btn_save_package_clicked();
};

#endif //SPAPACKAGEBUILDERDIALOG_H
