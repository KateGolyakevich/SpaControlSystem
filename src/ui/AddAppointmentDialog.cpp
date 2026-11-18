#include "ui/AddAppointmentDialog.h"
#include "core/Exceptions.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDateTime>

AddAppointmentDialog::AddAppointmentDialog(SpaSystem* system, QWidget* parent)
    : QDialog(parent), spa_system(system) {
    setup_ui();
    populate_combo_boxes();
}

void AddAppointmentDialog::setup_ui() {
    setWindowTitle("Новая запись на СПА-сеанс");
    setFixedSize(450, 320);

    auto* main_layout = new QVBoxLayout(this);
    auto* form_layout = new QFormLayout();

    combo_clients = new QComboBox(this);
    combo_employees = new QComboBox(this);
    combo_rooms = new QComboBox(this);
    combo_services = new QComboBox(this);

    edit_datetime = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    edit_datetime->setDisplayFormat("yyyy-MM-dd HH:mm");
    edit_datetime->setCalendarPopup(true);

    form_layout->addRow("Клиент:", combo_clients);
    form_layout->addRow("Мастер/Специалист:", combo_employees);
    form_layout->addRow("Кабинет/СПА-зона:", combo_rooms);
    form_layout->addRow("Процедура/Пакет:", combo_services);
    form_layout->addRow("Дата и время начала:", edit_datetime);

    main_layout->addLayout(form_layout);

    auto* btn_layout = new QHBoxLayout();
    auto* btn_save = new QPushButton("Забронировать", this);
    auto* btn_cancel = new QPushButton("Отмена", this);

    btn_layout->addStretch();
    btn_layout->addWidget(btn_cancel);
    btn_layout->addWidget(btn_save);
    main_layout->addLayout(btn_layout);

    connect(btn_save, &QPushButton::clicked, this, &AddAppointmentDialog::on_btn_save_clicked);
    connect(btn_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void AddAppointmentDialog::populate_combo_boxes() {
    for (const auto& c : spa_system->get_client_repo().get_all()) {
        combo_clients->addItem(QString::fromStdString(c.get_name() + " (" + c.get_phone_number() + ")"), c.get_id());
    }
    for (const auto& e : spa_system->get_employee_repo().get_all()) {
        combo_employees->addItem(QString::fromStdString(e.get_name() + " [" + e.get_specialty() + "]"), e.get_id());
    }
    for (const auto& r : spa_system->get_room_repo().get_all()) {
        combo_rooms->addItem(QString::fromStdString(r.get_name() + " (" + r.get_type() + ")"), r.get_id());
    }
    for (const auto& s : spa_system->get_services()) {
        combo_services->addItem(QString::fromStdString(s->get_title() + " - " +
                                std::to_string(static_cast<int>(s->calculate_cost())) + " руб."), s->get_id());
    }
}

void AddAppointmentDialog::on_btn_save_clicked() {
    if (combo_clients->count() == 0 || combo_employees->count() == 0 ||
        combo_rooms->count() == 0 || combo_services->count() == 0) {
        QMessageBox::warning(this, "Внимание", "Недостаточно данных для создания записи!");
        return;
    }

    int client_id = combo_clients->currentData().toInt();
    int employee_id = combo_employees->currentData().toInt();
    int room_id = combo_rooms->currentData().toInt();
    int service_id = combo_services->currentData().toInt();
    std::string dt_str = edit_datetime->dateTime().toString("yyyy-MM-dd HH:mm").toStdString();

    try {
        spa_system->schedule_appointment(client_id, employee_id, room_id, service_id, dt_str);
        QMessageBox::information(this, "Успех", "Запись успешно добавлена в расписание!");
        accept();
    } catch (const ResourceCollisionException& e) {
        QMessageBox::critical(this, "Конфликт ресурсов (Киллер-фича)", e.what());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}