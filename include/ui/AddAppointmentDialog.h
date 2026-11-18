#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include "core/SpaSystem.h"

class AddAppointmentDialog : public QDialog {
    Q_OBJECT

private:
    SpaSystem* spa_system;

    QComboBox* combo_clients;
    QComboBox* combo_employees;
    QComboBox* combo_rooms;
    QComboBox* combo_services;
    QDateTimeEdit* edit_datetime;

    void setup_ui();
    void populate_combo_boxes();

public:
    explicit AddAppointmentDialog(SpaSystem* system, QWidget* parent = nullptr);
    ~AddAppointmentDialog() override = default;

private slots:
    void on_btn_save_clicked();
};

#endif //ADDAPPOINTMENTDIALOG_H
