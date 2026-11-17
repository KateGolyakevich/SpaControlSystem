#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include "core/SpaSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddAppointmentDialog; }
QT_END_NAMESPACE

class AddAppointmentDialog : public QDialog {
    Q_OBJECT

private:
    Ui::AddAppointmentDialog* ui;
    SpaSystem* spa_system; // Указатель на ядро системы для доступа к данным

    // Заполнение выпадающих списков (ComboBox) реальными данными из репозиториев
    void populate_combo_boxes();

public:
    explicit AddAppointmentDialog(SpaSystem* system, QWidget* parent = nullptr);
    ~AddAppointmentDialog() override;

private slots:
    void on_btn_save_clicked();   // Обработка кнопки "Забронировать"
    void on_btn_cancel_clicked(); // Закрытие диалога
};

#endif //ADDAPPOINTMENTDIALOG_H
