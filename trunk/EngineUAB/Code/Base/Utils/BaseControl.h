#pragma once

#ifndef __BASE_CONTROL_H__
#define __BASE_CONTROL_H__

/**
 * Classe de control.
 * Aquesta classe defineix un patr� de funcionament que seguiran la resta de classes de l'aplicaci�.
 * Aquest patr� vindr� determinat pels m�todes IsOK(), Done(), Release() i SetOk().
**/
class CBaseControl
{
public:
  /**
   * Constructor per defecte.
   * La classe es crea per� no es troba en Ok.
  **/
                        CBaseControl():m_bIsOk    (false)         {};
  /**
   * Destructor.
   * Destructor de la classe. Per tal d'alliberar fer servir Done().
   * @see Done()
  **/
  virtual               ~CBaseControl             ()              {};
 
  /**
   * M�tode per alliberar els recursos.
   * Aquest m�tode allibera els recursos de la classe i la deixa com a no ok.
   * @see Release()
  **/
  void                  Done                      ()              {if(IsOk()) {this->Release(); m_bIsOk=false;}};
  /**
   * M�tode per comprovar si la classe s'ha inicialitzat correctament.
   * En cas que la classe s'hagi iniciat correctament, estar� en Ok.
   * @return L'estat de la classe.
  **/
  bool                  IsOk                      () const        {return m_bIsOk;};
 
private:
  /**
   * Variable interna de control de l'estat de la classe.
  **/
  bool                  m_bIsOk;
protected:
  /**
   * M�tode per alliberar recursos.
   * Cada classe que derivi d'aquesta haur� d'implementar aquest m�tode. D'aquesta forma es comprova un correcte alliberament de recursos.
  **/
  virtual void          Release                   ()=0;
  /**
   * M�tode per gestionar l'estat de la classe.
   * M�tode mitjan�ant el qual les classes derivades podran determinar el seu estat.
  **/
  void                  SetOk                     (bool _bIsOk)   {m_bIsOk = _bIsOk;};
};

#endif