#ifndef DHTML_EVENT_H
#define DHTML_EVENT_H

template <class T> class CHtmlEventObject : public IDispatch  
{

typedef void (T::*EVENTFUNCTIONCALLBACK)(DISPID id, VARIANT* pVarResult);

public:
   CHtmlEventObject() { m_cRef = 0; }	
   ~CHtmlEventObject() {}

   HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject)
   {
      *ppvObject = NULL;

      if (IsEqualGUID(riid, IID_IUnknown))
         *ppvObject = reinterpret_cast<void**>(this);

      if (IsEqualGUID(riid, IID_IDispatch))
         *ppvObject = reinterpret_cast<void**>(this);

      if (*ppvObject)
      {
         ((IUnknown*)*ppvObject)->AddRef();
         return S_OK;
      }
      else return E_NOINTERFACE;
   }

   DWORD __stdcall AddRef()
   {
      return InterlockedIncrement(&m_cRef);
   }

   DWORD __stdcall Release()
   {
      if (InterlockedDecrement(&m_cRef) == 0)
      {
         delete this;
         return 0;
      }
      return m_cRef;
   }

   STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pctinfo)
   { return E_NOTIMPL; }

   STDMETHOD(GetTypeInfo)(unsigned int iTInfo, LCID  lcid, ITypeInfo FAR* FAR*  ppTInfo)
   { return E_NOTIMPL; }

   STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId)
   { return S_OK; }

   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid,
      WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult,
      EXCEPINFO * pExcepInfo, UINT * puArgErr)
   {
      if (DISPID_VALUE == dispIdMember)
         (m_pT->*m_pFunc)(m_id, pVarResult);
      else
      {
         AtlTrace(_T("Invoke dispid = %d\n"), dispIdMember);
      }
	
      return S_OK;
   }

public:
   static LPDISPATCH CreateHandler(T* pT,
                        EVENTFUNCTIONCALLBACK pFunc, DISPID id)
   {
      CHtmlEventObject<T>* pFO = new CHtmlEventObject<T>;
      pFO->m_pT = pT;
      pFO->m_pFunc = pFunc;
      pFO->m_id = id;
      return reinterpret_cast<LPDISPATCH>(pFO);
   }

protected:
   T* m_pT;
   EVENTFUNCTIONCALLBACK m_pFunc;
   DISPID m_id;
   long m_cRef;
};

#endif DHTML_EVENT_H