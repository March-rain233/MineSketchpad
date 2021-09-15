#include "DrawToolUIFactory.h"
#include "ToolFactory.h"
#include "PencilUI.h"
#include "FilterPenUI.h"
#include "SpectroscopeUI.h"

DrawToolUIFactory* DrawToolUIFactory::_instance = nullptr;

DrawToolUIFactory& DrawToolUIFactory::GetInstance() {
    if (_instance == nullptr) {
        _instance = new DrawToolUIFactory();
    }
    return *_instance;
}

DrawToolsUI* DrawToolUIFactory::Create(QString name, QWidget* parent) {
    if (_pool.contains(name)) {
        _pool[name]->setParent(parent);
        return _pool[name];
    }

    DrawTools* tool = ToolFactory::GetInstance().GetTool(name);
    DrawToolsUI* res = nullptr;
    if (name == "Pencil") {
        res = new PencilUI(parent);
        res->_tool = tool;
    }
    else if(name == "Eraser") {
        res = new PencilUI(parent);
        res->_tool = tool;
    }
    else if(name == "MoveHand") {
        res = new DrawToolsUI(parent);
        res->_tool = tool;
    }
    else if(name == "FilterPen") {
        res = new FilterPenUI(parent);
        res->_tool = tool;
    }
    else if (name == "Spectroscope") {
        res = new SpectroscopeUI(parent);
        res->_tool = tool;
    }
    res->Init();

    _pool.insert(name, res);
    return res;
}
