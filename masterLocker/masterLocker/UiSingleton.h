#ifndef UISINGLETON_H
#define UISINGLETON_H

#include <QPointer>
#include <QMutex>

/**  类名：CUiSingleton
	  *  说明：单件类
	 **/
	template <class T>
	class CUiSingleton
	{
	public:
		// 访问单件的唯一方式
		static T& Instance();

	protected:
		// 构造
		CUiSingleton() {}
		// 析够
		virtual ~CUiSingleton() {}
	};

	/** @fn	    inline T& CUiSingleton<T>::Instance()
	 *  @brief	获取实例的接口
	 *  @param	void
	 *  @return	T& 对象的引用
	 */
	template <class T>
	inline T& CUiSingleton<T>::Instance()
	{
        static std::auto_ptr<T> s_instance;
		static QMutex m_mutex;

		if (s_instance.get() == NULL)
		{
			QMutexLocker locker(&m_mutex); //! 加锁避免同时被创建两个实例
									   //! CUiSingleton<T>作用域下互斥量，不需要担心不同类型之间的互斥
			if (s_instance.get() == NULL) // 第二次判断的目的在于不被多线程重复创建
			{
				s_instance = std::auto_ptr<T>(new T);
			}
		}

		return *s_instance.get();
	}

    /**  类名：CObjectSingleton
    *  说明：窗口单件类
    *  使用此单件需了解Qt的父对象销毁子对象机制以及父对象何时会改变
    *  需遵循Qt线程安全机制
    *  QObject单件可以在子线程中创建,但QWidget及其子类需在主线程中使用。
    *  各自继承实现的接口需考虑线程安全。QObject单件不保证线程安全
    **/
    template <class T>
    class CObjectSingleton
    {
    public:
        // 创建窗口单件方式
        // 若参数输入为空，需手动销毁单件
        // 若参数输入父对象，则此单件会在其父对象销毁自动销毁，用户可以考虑不需要手动删除单件
        static T* CreateInstance(QObject* pParent);

        // 创建窗口单件方式
        // 若参数2输入为空，需手动销毁单件
        // 若参数2输入父对象，则此单件会在其父对象销毁自动销毁，用户可以考虑不需要手动删除单件
        // 参数1可传入指定参数构造
        template <class U>
        static T* CreateInstance(U u, QObject* pParent)
        {
            // 如果单件不存在，创建单件
            if (s_instance.isNull())
            {
                s_instance = new T(u, pParent);
            }

            // 返回创建的窗口单件
            return s_instance;
        }
        // 创建窗口单件唯一方式
        // 此接口不指定父对象，满足Qt不同父对象要求
        static T* CreateInstance();
        // 访问窗口单件唯一方式，需保证其单件已经创建
        // 尽量在其生命期内调用
        static T* Instance();

        // 手动销毁单件
        // 若指定父对象可以不用调用此接口
        static void DeleteInstance();

    protected:
        // 构造
        CObjectSingleton(QObject* pParent){}
        // 析够
        virtual ~CObjectSingleton() {}

    private:
        static QPointer <T> s_instance; // 窗口单件，智能指针方式保存
    };

    template <class T>
    QPointer <T> CObjectSingleton<T>::s_instance = 0;

    /** @fn    inline T* CObjectSingleton<T>::CreateInstance(QObject* pParent)
    * @brief 创建窗口单件，若存在不创建
    * @param <IN> QObject* pParent - 父对象.
    * @return T - 模板对象.
    */

    template <class T>
    inline T* CObjectSingleton<T>::CreateInstance(QObject* pParent)
    {
        // 如果单件不存在，创建单件
        if (s_instance.isNull())
        {
            s_instance = new T(pParent);
        }

        // 返回创建的窗口单件
        return s_instance;
    }

    /** @fn    inline T* CObjectSingleton<T>::CreateInstance()
    * @brief 创建窗口单件，若存在不创建，需手动销毁
    * @param <IN> none.
    * @return T - 模板对象.
    */
    template <class T>
    inline T* CObjectSingleton<T>::CreateInstance()
    {
        // 如果单件不存在，创建单件
        if (s_instance.isNull())
        {
            s_instance = new T;
        }

        return s_instance;
    }

    /** @fn    inline T* CObjectSingleton<T>::Instance()
    * @brief 访问窗口单件唯一方式 需保证已经创建
    * @param <IN> none.
    * @return T - 模板对象.
    */
    template <class T>
    inline T* CObjectSingleton<T>::Instance()
    {
        return s_instance;
    }

    /** @fn    inline void CObjectSingleton<T>::DeleteInstance()
    * @brief 访问窗口单件唯一方式 需保证已经创建
    * @param <IN> none.
    * @return T - 模板对象.
    */
    template <class T>
    inline void CObjectSingleton<T>::DeleteInstance()
    {
        // 如果单件不存在，创建单件
        if (!s_instance.isNull())
        {
            delete s_instance;
            s_instance = NULL;
        }
    }

#endif // UISINGLETON_H
