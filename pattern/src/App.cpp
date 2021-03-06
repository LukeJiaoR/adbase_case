#include <adbase/Utility.hpp>
#include <adbase/Logging.hpp>
#include "App.hpp"

//{{{ macros

#define LOAD_KAFKA_CONSUMER_CONFIG(name, sectionName) do {\
    _configure->isNewConsumer##name = config.getOptionBool("kafkac_"#sectionName, "isNewConsumer"#name);\
    _configure->topicNameConsumer##name    = config.getOption("kafkac_"#sectionName, "topicName"#name);\
    _configure->groupId##name      = config.getOption("kafkac_"#sectionName, "groupId"#name);\
    _configure->brokerListConsumer##name   = config.getOption("kafkac_"#sectionName, "brokerList"#name);\
    _configure->kafkaDebug##name   = config.getOption("kafkac_"#sectionName, "kafkaDebug"#name);\
    _configure->offsetPath##name   = config.getOption("kafkac_"#sectionName, "offsetPath"#name);\
    _configure->statInterval##name = config.getOption("kafkac_"#sectionName, "statInterval"#name);\
} while(0)

#define LOAD_KAFKA_PRODUCER_CONFIG(name, sectionName) do {\
    _configure->topicNameProducer##name    = config.getOption("kafkap_"#sectionName, "topicName"#name);\
    _configure->brokerListProducer##name   = config.getOption("kafkap_"#sectionName, "brokerList"#name);\
    _configure->debug##name        = config.getOption("kafkap_"#sectionName, "debug"#name);\
    _configure->queueLength##name  = config.getOptionUint32("kafkap_"#sectionName, "queueLength"#name);\
} while(0)

#define LOAD_TIMER_CONFIG(name) do {\
	_configure->interval##name = config.getOptionUint32("timer", "interval"#name);\
} while(0)

//}}}
// {{{ App::App()

App::App(AdbaseConfig* config) :
	_configure(config) {
}

// }}}
// {{{ App::~App()

App::~App() {
}

// }}}
// {{{ void App::run()

void App::run() {
    _patternManager = new app::PatternManager(_configure);
    _patternManager->init();
}

// }}}
// {{{ void App::reload()

void App::reload() {
}

// }}}
// {{{ void App::stop()

void App::stop() {
    if (_patternManager != nullptr) {
        delete _patternManager;
        _patternManager = nullptr;
    }
}

// }}}
// {{{ void App::setAdServerContext()

void App::setAdServerContext(AdServerContext* context) {
	context->app = this;
    context->patternManager = _patternManager;
}

// }}}
// {{{ void App::setTimerContext()

void App::setTimerContext(TimerContext* context) {
	context->app = this;
}

// }}}
//{{{ void App::loadConfig()

void App::loadConfig(adbase::IniConfig& config) {
    // 解析词库配置
    std::vector<std::string> patternKeys  = config.options("pattern");
    std::unordered_map<std::string, std::string> patternConfig; 
    for (auto & t : patternKeys) {
        patternConfig[t] = config.getOption("pattern", t);
    }
    _configure->patternConfig = patternConfig;
}

//}}}
//{{{ uint64_t App::getSeqId()

uint64_t App::getSeqId() {
	std::lock_guard<std::mutex> lk(_mut);
	adbase::Sequence seq;
	return seq.getSeqId(static_cast<uint16_t>(_configure->macid), static_cast<uint16_t>(_configure->appid));
}

//}}}
