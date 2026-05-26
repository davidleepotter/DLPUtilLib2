/*********************************************************************
*	File :	objectcount.h 
*	Desc :	Base Template Class to count object instantiation
*			
*
*
*	Date: 3/25/2004
********************************************************************/

template<typename T>
class CObjectCount  
{
	private:
		static size_t objectcount;

	public:
		CObjectCount()
		{
			objectcount++;
		}

		CObjectCount(const CObjectCount&)
		{
			objectcount++;
		}

		virtual ~CObjectCount()
		{
			objectcount--;
		}

		static size_t GetObjectCount(void)
		{
			return objectcount;
		}
};

template<typename T>
size_t
CObjectCount<T>::objectcount = 0; // this now can go in header
