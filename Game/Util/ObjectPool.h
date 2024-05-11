#pragma once
#include "Util/IObjectPoolable.h"

class ObjectPool
{
public:
    std::multimap<std::string, IObjectPoolable*> PoolObjects;
    std::vector<IObjectPoolable*> vec_PoolObjects;
public:
    template<typename T>
    T* GetRecycledObject(std::string name)
    {
        T* poolObj = nullptr;

        if (PoolObjects.empty())// empty? than add new one
        {
            PoolObjects.emplace(name, dynamic_cast<IObjectPoolable*>(poolObj = new T));
            return poolObj;
        }

        for (auto elem : PoolObjects)// recycle old one
        {
            if (elem.second->CanRecyclable)
            {
                if (elem.first == name) {
                    poolObj = static_cast<T*>(elem.second);
                    break;
                }
            }
        }

        if (poolObj == nullptr)// using every resources now.. so add new one
        {
            PoolObjects.emplace(name, dynamic_cast<IObjectPoolable*>(poolObj = new T));
            return poolObj;
        }

        poolObj->OnRecycle();

        return poolObj;
    }

    template<typename T>
    T* GetRecycledObject()
    {
        T* poolObj = nullptr;

        if (vec_PoolObjects.empty())// empty? than add new one
        {
            vec_PoolObjects.push_back(dynamic_cast<IObjectPoolable*>(poolObj = new T));
            return poolObj;
        }

        for (auto elem : vec_PoolObjects)// recycle old one
        {
            if (elem->CanRecyclable)
            {
                poolObj = static_cast<T*>(elem);
                break;
            }
        }

        if (poolObj == nullptr)// using every resources now.. so add new one
        {
            vec_PoolObjects.push_back(dynamic_cast<IObjectPoolable*>(poolObj = new T));
            return poolObj;
        }

        poolObj->OnRecycle();

        return poolObj;
    }
};

