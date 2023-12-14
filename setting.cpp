#include "setting.h"
#include "srcnames.h"
#include "mvmeters.h"
#include "mvlayout.h"
#include "marker.h"
#include "micinputs.h"
#include "record.h"
#include "srcselections.h"
#include "outformat.h"
#include "outsources.h"
#include "panel.h"
#include "language.h"
#include "network.h"

Setting::Setting(QObject *parent) : QObject(parent)
{
    m_srcNames = new SrcNames(this);
    m_srcNames->setObjectName("srcNames");

    m_mvMeters = new MvMeters(this);
    m_mvMeters->setObjectName("mvMeters");

    m_mvLayout = new MvLayout(this);
    m_mvLayout->setObjectName("mvLayout");

    m_marker = new Marker(this);
    m_marker->setObjectName("marker");

    m_micInputs = new MicInputs(this);
    m_micInputs->setObjectName("micInputs");

    m_record = new Record(this);
    m_record->setObjectName("record");

    m_srcSelections = new SrcSelections(this);
    m_srcSelections->setObjectName("srcSelections");

    m_outFormat = new OutFormat(this);
    m_outFormat->setObjectName("outFormat");

    m_outSources = new OutSources(this);
    m_outSources->setObjectName("outSources");

    m_panel = new Panel(this);
    m_panel->setObjectName("panel");

    m_language = new Language(this);
    m_language->setObjectName("language");

    m_network = new Network(this);
    m_network->setObjectName("network");
}
