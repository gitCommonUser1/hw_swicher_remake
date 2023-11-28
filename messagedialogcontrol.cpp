#include "messagedialogcontrol.h"
#include "hw_config.h"
#include "models.h"
#include "MenuEnums.h"
#include "rv_switch_api.h"
#include <QEventLoop>
#include <QApplication>

extern Models *models;

MessageDialogControl::MessageDialogControl(QObject *parent) : QObject(parent)
{
    setMessageDialogVisible(0);
    setProgressBarVisible(0);
    setProgressPercent(0);
}

void MessageDialogControl::dialogShow(QString p_title, QList<QString> p_options,int type)
{
    setProgressBarVisible(0);

    setType(type);
    setTitle(p_title);
    setOptions(p_options);
    setOptionCurrent(0);

    setMessageDialogVisible(1);
}

void MessageDialogControl::buttonClicked()
{
    if(m_options.size() == 0 || m_options.size() <= m_optionCurrent)
        return ;

    QString option = m_options[m_optionCurrent];

    switch (m_type) {
    case MESSAGE_NORMAL:
        break;
    case MESSAGE_FACTORY_RESET:
        if(m_optionCurrent != 0){
            // ok!
            if(settings->recordLedStatus() != E_STATUS_FAILED){
                dialogShow(tr("You are recording, please end the recording first."),{"Ok"});
                int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
                    if(index != settings->reallyOutFormat())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,settings->reallyOutFormat());
                return ;
            }else{
                models->resetFactory();
            }
//            dialogShow("Resetting is done, please restart the device.",{"Ok"});
        }
        break;
    case MESSAGE_SD_FORMAT:
        if(m_optionCurrent != 0){
            // ok!
            models->sd_format();
            return ;
        }
    case MESSAGE_OUT_FORMAT:
        if(m_optionCurrent != 0){
            // ok!
            if(settings->recordLedStatus() != E_STATUS_FAILED){
                dialogShow(tr("You are recording, please end the recording first."),{"Ok"});
                int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
                    if(index != settings->reallyOutFormat())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,settings->reallyOutFormat());
                return ;
            }else{
                models->setOutFormat();
                system("killall qt_monitor.sh");
                system("reboot");
            }
        }else{
            // cancel
            int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
                if(index != settings->reallyOutFormat())
                    settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,settings->reallyOutFormat());
        }
        break;
    }

    setMessageDialogVisible(0);
}

void MessageDialogControl::cancel()
{
    switch (m_type) {
    case MESSAGE_OUT_FORMAT:
        int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
            if(index != settings->reallyOutFormat())
                settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,settings->reallyOutFormat());
        break;
    }

    setMessageDialogVisible(0);
    emit reject();
}

void MessageDialogControl::setProcessPercent(int per)
{
    if(per < 0 || per > 100)
        return ;

    setTitle("");
    setOptions({});
    setProgressBarVisible(1);
    setProgressPercent(per);
    if(per == 100)
    {
        setOptions({tr("Close")});
        setOptionCurrent(0);
    }
}

void MessageDialogControl::setProcessOver()
{
    setProgressBarVisible(0);
}
