#include "ToolFactory.h"
#include "Pencil.h"

ToolFactory* ToolFactory::_instance = nullptr;

ToolFactory& ToolFactory::GetInstance() {
    if (_instance == nullptr) {
        _instance = new ToolFactory();
    }
    return *_instance;
}

DrawTools* ToolFactory::GetTool(QString name) {
    if (_pool.contains(name)) {
        return _pool[name];
    }
    else {
        auto res = Create(name);
        if (res != nullptr) {
            _pool.insert(name, res);
        }
        return res;
    }
}

ToolFactory::ToolFactory() {}

DrawTools* ToolFactory::Create(QString name) {
    DrawTools* res = nullptr;
    if (name == "Pencil") {
        Pencil * p = new Pencil();
        p->SetRadius(3);
        p->SetColor(MyImage::RGBQUAD{ 0,0,0,255 });
        res = p;
    }
    return res;
}
