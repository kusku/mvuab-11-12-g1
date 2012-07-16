#ifndef __CLASS_TELEGRAM_H__
#define __CLASS_TELEGRAM_H__

#include <iostream>
#include <math.h>

struct STelegram
{
	// La entidad que envia el telegrama
	int Sender;

	// la entidad que recibe el telegrama
	int Receiver;
	
	// El mensaje. Existe un enumerado de las opciones
	int Msg;
	
	// Mensajes pueden ser mostrados de inmediato o pasado un cierto tiempo. Si se requiere retardo este campo se guarda con el tiempo que el mensaje debería ser guardado
	double DispatchTime;
	
	// Info adicional que podria acompañar el mensaje 
	void* ExtraInfo;

	STelegram()
		: DispatchTime	(-1)
		, Sender		(-1)
		, Receiver		(-1)
		, Msg			(-1)
	{}


	STelegram( double _Time, int _Sender, int _Receiver, int _Msg, void*  _Info = NULL )
		: DispatchTime	(_Time)
		, Sender			(_Sender)
		, Receiver		(_Receiver)
		, Msg				(_Msg)
		, ExtraInfo		(_Info)
	{}

};

// Parámetro que indica el retardo menor posible para que 2 telegramas se consideren únicos
const double SmallestDelay = 0.25;

inline bool operator==(const STelegram& t1, const STelegram& t2)
{
  return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
          (t1.Sender == t2.Sender)        &&
          (t1.Receiver == t2.Receiver)    &&
          (t1.Msg == t2.Msg);
}

inline bool operator<(const STelegram& t1, const STelegram& t2)
{
  if (t1 == t2)
  {
    return false;
  }

  else
  {
    return  (t1.DispatchTime < t2.DispatchTime);
  }
}

inline std::ostream& operator<<(std::ostream& os, const STelegram& t)
{
  os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
     << "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;

  return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}

#endif //__CLASS_TELEGRAM_H__