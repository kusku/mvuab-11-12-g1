#pragma once

#ifndef _CORE_H
#define _CORE_H

class CCore
{
private:
public:
	CCore();
	virtual ~CCore();

	void Init();
	void Update(float ElapsedTime);
	void Render();
};

#endif