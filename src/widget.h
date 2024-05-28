#pragma once

#include <QWidget>
#include <QPixmap>
#include <QList>
#include <QLabel>

#include "clicklabel.h"
#include "loginBox.h"

namespace Ui
{
    class widget;
}

class widget : public QWidget
{
    Q_OBJECT

    private:
        Ui::widget* ui;
        QList<QPixmap*> m_Pic;
        QList<clickLabel*> m_Icons;
        loginBox* m_Login;
        loginType m_UserType;

    public:
        explicit widget(QWidget* parent = nullptr);
        ~widget();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void funcOfStudentManagement(const clickLabel* label);
        void reset();

    private:
        void initalWidget();
        void moveToCenter(QWidget& widget);
};
