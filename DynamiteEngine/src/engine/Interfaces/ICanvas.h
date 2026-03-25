#ifndef ICANVAS_H
#define ICANVAS_H

#include "IComponent.h"
#include "IPostionController.h"

class ICanvas : public IComponent, public IPositionController<ICanvas, ICanvas> {
public:
	ICanvas() = default;
	virtual ~ICanvas() = default;
};

#endif