#ifndef RT_CURRENTUSER_WX_H
#define RT_CURRENTUSER_WX_H

#include <vector>
#include <memory>

class TCurrent;
typedef std::shared_ptr<TCurrent> TCurrentPtr;
typedef std::vector<TCurrentPtr> TCurrentsArray;

class TCurrentUser
{
public:
    virtual ~TCurrentUser() {}
    virtual const TCurrentsArray& GetCurrents() const = 0;
    virtual void AddCurrent(TCurrentPtr current) = 0;
    virtual void RemoveCurrent(TCurrent* current) = 0;
};

#endif