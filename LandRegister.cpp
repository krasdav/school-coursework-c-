#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
/*  Program to manage Land register. Add lands , change owner, delete lands, list lands by address,
 *  list owners lands, count owners lands.
 *  For listing class Iterator is used.
 */
class COwner;
class CLand;
//===================================================================================================
// Functions to find lands/owners according the needs
bool                        CompareByCity                       ( shared_ptr<CLand>  X,
                                                                  shared_ptr<CLand>  Y );
bool                        CompareCityLow                      ( shared_ptr<CLand>  X,
                                                                  const string & city );
bool                        CompareCityUP                       ( const string & city,
                                                                  shared_ptr<CLand>  X );
bool                        CompareByReg                        ( shared_ptr<CLand>  X,
                                                                  shared_ptr<CLand>  Y );
bool                        CompareRegUP                        ( const string & region,
                                                                  shared_ptr<CLand>  X);

bool                        CompareRegLow                       ( shared_ptr<CLand>  X,
                                                                  const string & region);
bool                        EqualOwners                         ( shared_ptr<COwner> a,
                                                                  const string b );
bool                        EqualOwners                         ( const string  a, const string b);
bool                        CompareOwners                       ( shared_ptr<COwner> X,
                                                                  const string & y );
bool                        CompareID                           ( shared_ptr<CLand>  X,
                                                                  unsigned int  id );
bool                        CompareAddr                         ( shared_ptr<CLand>  X,
                                                                  const string & addr );
string                      Lower                               ( string x );
//===================================================================================================
class CLand
{
public:
                                CLand                           ( const string         & city,
                                                                  const string         & addr,
                                                                  const string         & region,
                                                                  unsigned int           id );

    const string &              getCity                         () const ;
    const string &              getAddr                         () const ;
    const string &              getRegion                       () const ;
    unsigned int                getID                           () const ;
    const string &              getOwner                        () const ;
    int                         getIdxInOwnerList               () const ;
    weak_ptr<COwner> &          getOwnedBy                      () ;

    void                        setOwner                        ( const string          & owner);
    void                        setIdxInOwnerList               ( int                     idx );
    void                        setOwnedBy                      ( shared_ptr<COwner>    & X);

private:
    string                      m_City;
    string                      m_Addr;
    string                      m_Region;
    unsigned int                m_Id;
    string                      m_Owner;
    int                         m_IdxInOwnerList;     // Land's index in owner list, used for fast access
    weak_ptr<COwner>            m_OwnedBy;            // pointer to owner
};
//----------------------------------------------------------------------------------------
                            CLand::CLand                    ( const string         & city,
                                                              const string         & addr,
                                                              const string         & region,
                                                              unsigned int           id ):
                                    m_City   ( city ),
                                    m_Addr   ( addr ),
                                    m_Region ( region ),
                                    m_Id     ( id ),
                                    m_Owner  ( "" ),
                                    m_IdxInOwnerList ( 0 )
{
}
//----------------------------------------------------------------------------------------
void                        CLand::setIdxInOwnerList (int idx)          { m_IdxInOwnerList = idx; }
//----------------------------------------------------------------------------------------
void                        CLand::setOwner ( const string & owner )    {  m_Owner = owner; }
//----------------------------------------------------------------------------------------
weak_ptr<COwner> &          CLand::getOwnedBy()                         { return m_OwnedBy ; }
//----------------------------------------------------------------------------------------
void                        CLand::setOwnedBy( shared_ptr<COwner> & X ) { m_OwnedBy = X ; }
//----------------------------------------------------------------------------------------
int                         CLand::getIdxInOwnerList()            const { return m_IdxInOwnerList; }
//----------------------------------------------------------------------------------------
const string &              CLand::getAddr()                      const { return m_Addr; }
//----------------------------------------------------------------------------------------
const string &              CLand::getCity()                      const { return m_City; }
//----------------------------------------------------------------------------------------
const string &              CLand::getRegion()                    const { return m_Region; }
//----------------------------------------------------------------------------------------
unsigned int                CLand::getID()                        const { return m_Id; }
//----------------------------------------------------------------------------------------
const string &              CLand::getOwner()                     const { return m_Owner; }
//===================================================================================================
class COwner
{
public:
                                COwner                          () ;
                                COwner                          ( const string & Name);

    const string &              getName                         () const ;
    int                         getLandOwned                    () const ;
    vector<weak_ptr<CLand>> &   getLandListPtr                  () ;

    void                        addLandListPtr                  ( shared_ptr<CLand> & P);
    void                        printOwned                      ();
private:
    string                      m_Name;
    int                         m_OwnedLand;        // number of lands owned
    vector<weak_ptr<CLand>>     m_LandListPtr;      // list of lands owned by owner
};
//----------------------------------------------------------------------------------------
                            COwner::COwner() :
                                    m_Name      ("") ,
                                    m_OwnedLand (0)
{
}
//----------------------------------------------------------------------------------------
                            COwner::COwner( const string & Name ) :
                                    m_Name      (Name),
                                    m_OwnedLand (0)
{
}
//----------------------------------------------------------------------------------------
const string &              COwner::getName()                     const { return m_Name; }
//----------------------------------------------------------------------------------------
int                         COwner::getLandOwned()                const { return m_OwnedLand; }
//----------------------------------------------------------------------------------------
vector<weak_ptr<CLand>> &   COwner::getLandListPtr()                    { return m_LandListPtr; }
//----------------------------------------------------------------------------------------
void                        COwner::addLandListPtr( shared_ptr<CLand> & P)
                                                                        { m_LandListPtr.push_back(P),
                                                                                    m_OwnedLand++ ; }
//----------------------------------------------------------------------------------------
void                        COwner::printOwned                      ()
{
    cout << " OWNED BY " << m_Name << endl;
    for( int i = 0 ; i < m_OwnedLand ; ++i)
    {
        while(m_LandListPtr[i].lock() == nullptr )
            i++;
        cout << " CITY : " <<  m_LandListPtr[i].lock()->getCity() << " ADDR " << m_LandListPtr[i].lock()->getAddr() << endl;
    }
}
//===================================================================================================
class CIterator
{
public:
                                CIterator                      ( const vector<shared_ptr<CLand>> & X,
                                                                 int LandNum);
                                CIterator                      ();
                                CIterator                      ( const shared_ptr<COwner> & X );
    bool                        AtEnd                          ( void ) const;
    void                        Next                           ( void );
    string                      City                           ( void ) const;
    string                      Addr                           ( void ) const;
    string                      Region                         ( void ) const;
    unsigned                    ID                             ( void ) const;
    string                      Owner                          ( void ) const;
private:
    vector<shared_ptr<CLand>>   Lands;   // list of Lands to iterate through
    vector<weak_ptr<CLand>>     weakPtrLands;
    int                         Flag;    // if owners Land list flag = 1, else 0
    int                         Idx;     // current Land
    int                         LandNR;  // number of Lands recieved
};
//----------------------------------------------------------------------------------------
                                    CIterator::CIterator           ( const vector<shared_ptr<CLand>>
                                                                     & X, int LandNum):
                                            Lands(X),
                                            Flag(0),
                                            Idx(0),
                                            LandNR(LandNum)
{
}
//----------------------------------------------------------------------------------------
                                    CIterator::CIterator                      ():
                                            Idx(0),
                                            LandNR(0)
{
}
//----------------------------------------------------------------------------------------
                                    CIterator::CIterator                      ( const shared_ptr<COwner> & X ):
                                            weakPtrLands(X->getLandListPtr()),
                                            Flag(1),
                                            Idx(0),
                                            LandNR(X->getLandOwned())
{
    while (!weakPtrLands[Idx].lock() && Idx < LandNR) // if its an empty pointer , jump indexes
        Idx++;
}
//----------------------------------------------------------------------------------------

bool                            CIterator::AtEnd                          ( void ) const
{
    return Idx == LandNR;
}
//----------------------------------------------------------------------------------------
void                            CIterator::Next                           ( void )
{
    Idx++;
    if ( Flag ) {
        auto it = weakPtrLands.begin() + Idx;
        if (it == weakPtrLands.end())
            return;

        while (!weakPtrLands[Idx].lock() && Idx <= LandNR) // If empty pointer, jump indexes
        {
            Idx++;
            auto it = weakPtrLands.begin() + Idx;
            if (it == weakPtrLands.end())
                return;
        }
    }
}
//----------------------------------------------------------------------------------------
string                          CIterator::City                           ( void ) const
{
    if( Flag )
        return weakPtrLands[Idx].lock()->getCity();
    else
        return Lands[Idx]->getCity();
}
//----------------------------------------------------------------------------------------
string                          CIterator::Addr                           ( void ) const
{
    if ( Flag )
        return weakPtrLands[Idx].lock()->getAddr();
    else
        return Lands[Idx]->getAddr();
}
//----------------------------------------------------------------------------------------
string                          CIterator::Region                         ( void ) const
{
    if( Flag )
        return weakPtrLands[Idx].lock()->getRegion();
    else
        return Lands[Idx]->getRegion();
}
//----------------------------------------------------------------------------------------
unsigned                        CIterator::ID                             ( void ) const
{
    if ( Flag )
        return weakPtrLands[Idx].lock()->getID();
    else
        return Lands[Idx]->getID();
}
//----------------------------------------------------------------------------------------
string                          CIterator::Owner                          ( void ) const
{
    if ( Flag )
        return weakPtrLands[Idx].lock()->getOwner();
    else
        return Lands[Idx]->getOwner();
}
//===================================================================================================
class CLandRegister
{
public:

    CLandRegister                   ();
    bool                        Add                             ( const string    & city,
                                                                  const string    & addr,
                                                                  const string    & region,
                                                                  unsigned int      id );

    bool                     Del                                ( const string    & city,
                                                                  const string    & addr );

    bool                     Del                                ( const string    & region,
                                                                  unsigned int      id );

    bool                     GetOwner                           ( const string    & city,
                                                                  const string    & addr,
                                                                  string          & owner ) const;

    bool                     GetOwner                           ( const string    & region,
                                                                  unsigned int      id,
                                                                  string          & owner ) const;

    bool                     NewOwner                           ( const string    & city,
                                                                  const string    & addr,
                                                                  const string    & owner );

    bool                     NewOwner                           ( const string    & region,
                                                                  unsigned int      id,
                                                                  const string    & owner );

    unsigned                 Count                              ( const string    & owner ) const;

    CIterator                ListByAddr                         ( void ) const;

    CIterator                ListByOwner                        ( const string    & owner ) const;

    bool                     AddToCityLandList                  ( shared_ptr<CLand> & X );
    bool                     AddToRegLandList                   ( shared_ptr<CLand> & X );
    bool                     EqualLands                         ( CLand & X ,
                                                                  CLand & Y);
    bool                     EqualLandsReg                      ( CLand & X ,
                                                                  CLand & Y);

    int                      getOwnerIdx                        ( const string own );
    void                     ChangeOwnerReg                     ( int O_idx , int L_idx,
                                                                  const string & owner );
    void                     ChangeOwnerCity                    ( int O_idx , int L_idx ,
                                                                  const string & owner );
    void                     printOwnerList                     ();
    void                     printLandList                      ();
    void                     printLandListReg                   ();
private:
    vector<shared_ptr<CLand>>   m_LandList;          // list sorted by City & addr
    vector<shared_ptr<COwner>>  m_OwnerList;         // list of Owners
    vector<shared_ptr<CLand>>   m_LandListByReg;     // list sorted by Region & id
    int                         m_LandNR;            // Number of Lands
    int                         m_OwnerNR;           // Number of Owners
};
//----------------------------------------------------------------------------------------
                            CLandRegister::CLandRegister                  ():
                                    m_LandNR    (0),
                                    m_OwnerNR   (0)
{
}
//----------------------------------------------------------------------------------------
bool                        CLandRegister::Add                              ( const string &city,
                                                                              const string &addr,
                                                                              const string &region,
                                                                              unsigned int  id )
{
    shared_ptr<CLand> Land = make_shared<CLand>(city,addr,region,id);
    return AddToCityLandList(Land);
}
//----------------------------------------------------------------------------------------
/*  @param X , Land created
 *  Function to add land to sorted list by City
 *  Creates owner if none exist
 *  returns false if equal land exists , otherwise returns true
 */
bool                        CLandRegister::AddToCityLandList            ( shared_ptr<CLand> & X )
{
    auto it = lower_bound( m_LandList.begin(),m_LandList.end(), X ,CompareByCity );
    if( it != m_LandList.end())
        if( EqualLands(**it,*X) )
            return false;
    auto at = lower_bound( m_LandListByReg.begin(),m_LandListByReg.end(), X ,CompareByReg );
    if( at != m_LandListByReg.end())
        if( EqualLandsReg(**at,*X) )
            return false;
    if( m_OwnerNR == 0 )
    {
        shared_ptr<COwner> O = make_shared<COwner>();
        m_OwnerList.push_back(O);
        X->setIdxInOwnerList(0);
        X->setOwnedBy(O);
        m_LandList.push_back(X);
        m_LandListByReg.push_back(X);
        O->addLandListPtr(X);
        m_OwnerNR++;
        m_LandNR++;
        return true;
    }

    m_LandList.insert(it,X);
    m_OwnerList.front()->addLandListPtr(X);
    X->setIdxInOwnerList(m_OwnerList[0]->getLandOwned()-1);
    X->setOwnedBy(m_OwnerList[0]);
    m_LandNR++;
    AddToRegLandList(X);

    return true;
}
//----------------------------------------------------------------------------------------
/* @param X , Land created
 * Inserts Land to list sorted by Region
 */
bool                        CLandRegister::AddToRegLandList                (shared_ptr<CLand> & X)
{
    auto it = lower_bound(m_LandListByReg.begin(),m_LandListByReg.end(),X,CompareByReg);
    m_LandListByReg.insert(it,X);
    return true;
}
//----------------------------------------------------------------------------------------
CIterator                   CLandRegister::ListByAddr                       () const
{
    CIterator itr(m_LandList,m_LandNR);
    return itr;
}
//----------------------------------------------------------------------------------------
CIterator                   CLandRegister::ListByOwner                      ( const string    & owner ) const
{
    const string own = Lower(owner);
    auto it = lower_bound(m_OwnerList.begin(),m_OwnerList.end(), own, CompareOwners);
    if( it == m_OwnerList.end()) // if owner not found construct empty Iterator with position atEnd
    {
        CIterator itr;
        return itr;
    }
    if( !EqualOwners((*it),own)) // if owner not found construct empty Iterator with position atEnd
    {
        CIterator itr;
        return itr;
    }

    CIterator itr(*it);
    return itr;
}
//----------------------------------------------------------------------------------------
unsigned                    CLandRegister::Count                           ( const string    & owner ) const {
    unsigned Owned = 0;
    string own = Lower(owner);
    auto it = lower_bound(m_OwnerList.begin(), m_OwnerList.end(), own, CompareOwners);
    // if owner not found return 0 owned Lands
    if (it == m_OwnerList.end())
    {
        return Owned;
    }
    if (!EqualOwners((*it), own))
    {
        return Owned;
    }
    // Count Lands owned by owner, jump through empty pointers
    for( int i = 0; i < (*it)->getLandOwned() ; i++) {
        if( !(*it)->getLandListPtr()[i].lock())
            continue;
        Owned++;
    }
    return Owned;
}
//----------------------------------------------------------------------------------------
/* @param, Own - name of searched owner
 * if found returns owner index in the list, if not returns new owner & his index
 */
int                     CLandRegister::getOwnerIdx                     ( const string Own )
{
    auto it_Owner = lower_bound(m_OwnerList.begin(), m_OwnerList.end(), Own, CompareOwners);
    int O_idx = distance(m_OwnerList.begin(),it_Owner);
    if( it_Owner == m_OwnerList.end())
    {
        shared_ptr<COwner> O = make_shared<COwner>(Own);
        m_OwnerList.push_back(O);
        m_OwnerNR ++;
    }else if( !EqualOwners((*it_Owner), Own))
    {
        shared_ptr<COwner> O = make_shared<COwner>(Own);
        m_OwnerList.insert(it_Owner,O);
        m_OwnerNR ++;
    }
    return O_idx;
}
//----------------------------------------------------------------------------------------
/* @param1 , O_idx , new owner index in the owners list
 * @param2 , L_idx , Land index
 * @param3 , owner , owners name
 */
void                     CLandRegister::ChangeOwnerCity              ( int O_idx , int L_idx ,
                                                                       const string & owner )
{
    //----set old owner ptr;
    m_LandList[L_idx]->getOwnedBy().lock()->getLandListPtr()[m_LandList[L_idx]->getIdxInOwnerList()].reset();
    //----set new Owner----
    m_OwnerList[O_idx]->addLandListPtr(m_LandList[L_idx]);
    //----set Land details of new owner;
    m_LandList[L_idx]->setIdxInOwnerList( m_OwnerList[O_idx]->getLandOwned()-1 );
    m_LandList[L_idx]->setOwnedBy( m_OwnerList[O_idx] );
    m_LandList[L_idx]->setOwner(owner);
}
void                     CLandRegister::ChangeOwnerReg               ( int O_idx , int L_idx ,
                                                                       const string & owner )
{
    //----set old owner ptr;
    m_LandListByReg[L_idx]->getOwnedBy().lock()->getLandListPtr()[m_LandListByReg[L_idx]->getIdxInOwnerList()].reset();
    //----set new Owner----
    m_OwnerList[O_idx]->addLandListPtr(m_LandListByReg[L_idx]);
    //----set Land details of new owner;
    m_LandListByReg[L_idx]->setIdxInOwnerList( m_OwnerList[O_idx]->getLandOwned()-1 );
    m_LandListByReg[L_idx]->setOwnedBy( m_OwnerList[O_idx] );
    m_LandListByReg[L_idx]->setOwner(owner);
}
//----------------------------------------------------------------------------------------
/* Fuction to set new owner
 * returns false if Land is not found or owner already owns the land, else true
 */
bool                     CLandRegister::NewOwner                      ( const string    & region,
                                                                        unsigned int      id,
                                                                        const string    & owner ) {
    const string Own = Lower(owner);
    int O_idx = getOwnerIdx(Own);

    auto it_low = lower_bound(m_LandListByReg.begin(),m_LandListByReg.end(),region, CompareRegLow);
    auto it_up = upper_bound(m_LandListByReg.begin(),m_LandListByReg.end(),region, CompareRegUP);

    if( it_low == m_LandListByReg.end()) {
        return false;
    }

    auto it_Land = lower_bound(it_low,it_up,id,CompareID );
    if ( it_Land == m_LandListByReg.end() || (*it_Land)->getID() != id)
        return false;

    int L_idx = distance(m_LandListByReg.begin(),it_Land);

    const string aa = Lower(m_LandListByReg[L_idx]->getOwner());
    if( EqualOwners(aa,Own))
        return false;

   ChangeOwnerReg(O_idx,L_idx,owner);
    return true;
}
//----------------------------------------------------------------------------------------
bool                     CLandRegister::NewOwner                      ( const string    & city,
                                                                        const string    & addr,
                                                                        const string    & owner )
{
    const string Own = Lower(owner);
    int O_idx = getOwnerIdx(Own);

    auto it_low = lower_bound(m_LandList.begin(),m_LandList.end(), city, CompareCityLow);
    auto it_up = upper_bound(m_LandList.begin(),m_LandList.end(), city, CompareCityUP);

    if( it_low == m_LandList.end())
        return false;

    auto it_Land = lower_bound(it_low,it_up,addr,CompareAddr);

    if( it_Land == m_LandList.end() || (*it_Land)->getAddr() != addr)
        return false;

    int L_idx = distance(m_LandList.begin(),it_Land);

    const string aa = Lower(m_LandList[L_idx]->getOwner());
    if( EqualOwners(aa,Own))
        return false;

    ChangeOwnerCity(O_idx,L_idx,owner);
    return true;
}
//----------------------------------------------------------------------------------------
/* Function to get Lands owner name
 * return false if Land is not found
 */
bool                     CLandRegister::GetOwner                      ( const string    & region,
                                                                        unsigned int      id,
                                                                        string          & owner ) const
{
    const string Own = Lower(owner);
    auto it_low = lower_bound(m_LandListByReg.begin(),m_LandListByReg.end(),region, CompareRegLow);
    auto it_up  = upper_bound(m_LandListByReg.begin(),m_LandListByReg.end(),region, CompareRegUP);
    if( it_low == m_LandListByReg.end())
        return false;

    auto it_Land = lower_bound(it_low,it_up,id, CompareID);
    if( it_Land == m_LandListByReg.end())
        return false;

    if( (*it_Land)->getRegion() !=region || (*it_Land)->getID() != id)
        return false;


    owner = (*it_Land)->getOwner();

    return true;
}
//----------------------------------------------------------------------------------------
bool                     CLandRegister::GetOwner                      ( const string    & city,
                                                                        const string    & addr,
                                                                        string          & owner ) const
{
    const string Own = Lower(owner);
    auto it_low = lower_bound(m_LandList.begin(),m_LandList.end(),city, CompareCityLow);
    auto it_up  = upper_bound(m_LandList.begin(),m_LandList.end(),city, CompareCityUP);
    if( it_low == m_LandList.end())
        return false;

    auto it_Land = lower_bound(it_low,it_up,addr, CompareAddr);
    if( it_Land == m_LandList.end())
        return false;
    if( (*it_Land)->getCity() != city || (*it_Land)->getAddr() != addr)
        return false;

    owner = (*it_Land)->getOwner();

    return true;
}
//----------------------------------------------------------------------------------------
bool                        CLandRegister::Del                          ( const string    & region,
                                                                          unsigned int      id )
{
    auto it_Land = lower_bound(m_LandListByReg.begin(),m_LandListByReg.end(),region, CompareRegLow);
    if( it_Land == m_LandListByReg.end())
        return false;
    while( (*it_Land)->getRegion() == region )
    {
        if( (*it_Land)->getID() == id)
        {
            //---- del owners ptr ----
            ((*it_Land)->getOwnedBy().lock()->getLandListPtr())[(*it_Land)->getIdxInOwnerList()].reset();
            //---- del Land from listCity ---
            auto it_city = lower_bound(m_LandList.begin(),m_LandList.end(),
                                       (*it_Land)->getCity(), CompareCityLow);
            while( (*it_city)->getCity() == (*it_Land)->getCity())
            {
                if( (*it_city)->getAddr() == (*it_Land)->getAddr())
                {
                    m_LandList.erase(it_city);
                    break;
                }
                it_city++;
            }
            //---- del Land from listReg ---
            m_LandListByReg.erase(it_Land);
            m_LandNR--;
            return true;
        }
        it_Land++;
        if( it_Land == m_LandListByReg.end())
            return false;
    }
    return false;
}
//----------------------------------------------------------------------------------------
bool                        CLandRegister::Del                          ( const string    & city,
                                                                          const string    & addr )
{
    auto it_Land = lower_bound(m_LandList.begin(),m_LandList.end(),city, CompareCityLow);
    if( it_Land == m_LandList.end())
        return false;
    while( (*it_Land)->getCity() == city )
    {
        if( (*it_Land)->getAddr() == addr)
        {
            //---- del owners ptr ----
            ((*it_Land)->getOwnedBy()).lock()->getLandListPtr() [ (*it_Land)->getIdxInOwnerList() ].reset();
            //---- del Land from listReg
            auto it_reg = lower_bound(m_LandListByReg.begin(),m_LandListByReg.end(),
                                      (*it_Land)->getRegion(), CompareRegLow);
            while( (*it_reg)->getRegion() == (*it_Land)->getRegion())
            {
                if( (*it_reg)->getID() == (*it_Land)->getID())
                {
                    m_LandListByReg.erase(it_reg);
                    break;
                }
                it_reg++;
            }
            //---- del Land from listCity ---
            m_LandList.erase(it_Land);
            m_LandNR--;
            return true;
        }
        it_Land++;
        if( it_Land == m_LandList.end())
            return false;
    }
    return false;
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void                        CLandRegister::printLandList                    ()
{
    cout << " <<< PRINT BY CITY >>> " << endl;
    for( int i = 0 ; i < m_LandNR ; ++i )
    {
        if( m_LandList[i] == nullptr)
            continue;
        cout << "|CITY|:" << m_LandList[i]->getCity()
             << "   " << "|ADDR|:" << m_LandList[i]->getAddr()
             << "   " << "|REG|:" << m_LandList[i]->getRegion()
             << "   " << "|ID|:" << m_LandList[i]->getID()
             << "   " << "|OWNER|:" << m_LandList[i]->getOwner()
             << "   " << "|OWNER_LIST_INDEX|:" << m_LandList[i]->getIdxInOwnerList()
             << "   " << "|OWNER_PTR|:" << m_LandList[i]->getOwnedBy().lock()->getName()
             << "   " << "|LandNR|:" << m_LandNR << endl;
    }
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void                        CLandRegister::printLandListReg                 ()
{
    cout << " <<< PRINT BY REG >>> " << endl;
    for( int i = 0 ; i < m_LandNR ; ++i )
    {
        if( m_LandListByReg[i] == nullptr)
            continue;
        cout << "|CITY|:" << m_LandListByReg[i]->getCity()
             << "   " << "|ADDR|:" << m_LandListByReg[i]->getAddr()
             << "   " << "|REG|:" << m_LandListByReg[i]->getRegion()
             << "   " << "|ID|:" << m_LandListByReg[i]->getID()
             << "   " << "|OWNER|:" << m_LandListByReg[i]->getOwner()
             << "   " << "|OWNER_LIST_INDEX|:" << m_LandListByReg[i]->getIdxInOwnerList()
             << "   " << "|OWNER_PTR|:" << m_LandListByReg[i]->getOwnedBy().lock()->getName()
             << "   " << "|LandNR|:" << m_LandNR << endl;
    }
}
void                        CLandRegister::printOwnerList                  ()
{
    for( int i = 0 ; i < m_OwnerNR ; i++)
    {
        cout << " OWNER" << i << "  " << m_OwnerList[i]->getName() << endl;
        for( int s = 0 ; s < m_OwnerList[i]->getLandOwned(); ++s)
        {
            if( m_OwnerList[i]->getLandListPtr()[s].lock() == nullptr)
                continue;
            cout << "CITY: " << m_OwnerList[i]->getLandListPtr()[s].lock()->getCity()
                 << "  ADDR: " << m_OwnerList[i]->getLandListPtr()[s].lock()->getAddr()
                 << "  REGIO: " << m_OwnerList[i]->getLandListPtr()[s].lock()->getRegion()
                 << "  ID: " << m_OwnerList[i]->getLandListPtr()[s].lock()->getID()
                 << "  OWNER: " << m_OwnerList[i]->getLandListPtr()[s].lock()->getOwner() <<endl;
        }
    }
}
//----------------------------------------------------------------------------------------
bool                       CLandRegister::EqualLands                            ( CLand & X ,
                                                                                  CLand & Y)
{
    return ( X.getCity() == Y.getCity() ) && ( X.getAddr() == Y.getAddr());
}
//----------------------------------------------------------------------------------------
bool                       CLandRegister::EqualLandsReg                         ( CLand & X ,
                                                                                  CLand & Y)
{
    return ( X.getRegion() == Y.getRegion() ) && ( X.getID() == Y.getID());
}
//----------------------------------------------------------------------------------------
bool                      CompareRegLow                                         ( shared_ptr<CLand>  X,
                                                                                  const string & region )
{
    return (X->getRegion().compare(region) < 0);
}
//----------------------------------------------------------------------------------------

bool                      CompareRegUP                                          ( const string & region,
                                                                                  shared_ptr<CLand>  X)
{
    return (X->getRegion().compare(region) > 0);
}
//----------------------------------------------------------------------------------------

bool                      CompareCityLow                                        ( const shared_ptr<CLand>  X,
                                                                                  const string & city )
{
    return (X->getCity().compare(city) < 0);
}
//----------------------------------------------------------------------------------------

bool                      CompareCityUP                                         ( const string & city,
                                                                                  const shared_ptr<CLand>  X )
{
    return (X->getCity().compare(city) > 0);
}
//----------------------------------------------------------------------------------------
bool                        CompareByCity                                       ( shared_ptr<CLand> X,
                                                                                  const shared_ptr<CLand>  Y )
{
    if( X->getCity().compare(Y->getCity()) < 0 )
    {
        return true;
    }else if ( X->getCity() == Y->getCity() )
        return (X->getAddr().compare(Y->getAddr()) < 0);
    else
        return false;
}
//----------------------------------------------------------------------------------------
bool                        CompareByReg                                        ( shared_ptr<CLand> X,
                                                                                  const shared_ptr<CLand> Y )
{
    if( X->getRegion().compare(Y->getRegion()) < 0 )
    {
        return true;
    }else if ( X->getRegion() == Y->getRegion() )
        return (X->getID() < (Y->getID()) );
    else
        return false;
}

//----------------------------------------------------------------------------------------
bool                        CompareID                                           ( const shared_ptr<CLand>  X,
                                                                                  unsigned int  id )
{
    return X->getID() < id;
}
bool                        CompareAddr                                         ( const shared_ptr<CLand>  X,
                                                                                  const string & addr )
{
    return X->getAddr() < addr;
}
//----------------------------------------------------------------------------------------

bool                       EqualOwners                                          ( const shared_ptr<COwner> a,
                                                                                  const string b)
{
    return a->getName() == b;
}
//----------------------------------------------------------------------------------------
bool                       EqualOwners                                          ( const string  a,
                                                                                  const string b )
{
    return a == b;
}
//----------------------------------------------------------------------------------------
bool                        CompareOwners                                       ( shared_ptr<COwner> X,
                                                                                  const string & y )
{
    const string b = Lower(X->getName());
    return (b.compare(y) < 0);
}

string                      Lower                                               ( string x )
{
    transform(x.begin(),x.end(),x.begin(),::tolower);
    return x;
}


#ifndef __PROGTEST__
static void test0 ( void )
{
    CLandRegister x;
    string owner;

    assert ( x . Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
    assert ( x . Add ( "Prague", "Evropska", "Vokovice", 12345 ) );
    assert ( x . Add ( "Prague", "Technicka", "Dejvice", 9873 ) );
    assert ( x . Add ( "Plzen", "Evropska", "Plzen mesto", 78901 ) );
    assert ( x . Add ( "Liberec", "Evropska", "Librec", 4552 ) );
    CIterator i0 = x . ListByAddr ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Liberec"
             && i0 . Addr () == "Evropska"
             && i0 . Region () == "Librec"
             && i0 . ID () == 4552
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Plzen"
             && i0 . Addr () == "Evropska"
             && i0 . Region () == "Plzen mesto"
             && i0 . ID () == 78901
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Evropska"
             && i0 . Region () == "Vokovice"
             && i0 . ID () == 12345
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Technicka"
             && i0 . Region () == "Dejvice"
             && i0 . ID () == 9873
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Thakurova"
             && i0 . Region () == "Dejvice"
             && i0 . ID () == 12345
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( i0 . AtEnd () );

    assert ( x . Count ( "" ) == 5 );
    CIterator i1 = x . ListByOwner ( "" );
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Prague"
             && i1 . Addr () == "Thakurova"
             && i1 . Region () == "Dejvice"
             && i1 . ID () == 12345
             && i1 . Owner () == "" );
    i1 . Next ();
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Prague"
             && i1 . Addr () == "Evropska"
             && i1 . Region () == "Vokovice"
             && i1 . ID () == 12345
             && i1 . Owner () == "" );
    i1 . Next ();
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Prague"
             && i1 . Addr () == "Technicka"
             && i1 . Region () == "Dejvice"
             && i1 . ID () == 9873
             && i1 . Owner () == "" );
    i1 . Next ();
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Plzen"
             && i1 . Addr () == "Evropska"
             && i1 . Region () == "Plzen mesto"
             && i1 . ID () == 78901
             && i1 . Owner () == "" );
    i1 . Next ();
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Liberec"
             && i1 . Addr () == "Evropska"
             && i1 . Region () == "Librec"
             && i1 . ID () == 4552
             && i1 . Owner () == "" );
    i1 . Next ();
    assert ( i1 . AtEnd () );

    assert ( x . Count ( "CVUT" ) == 0 );
    CIterator i2 = x . ListByOwner ( "CVUT" );
    assert ( i2 . AtEnd () );
    assert ( x . NewOwner ( "Prague", "Thakurova", "CVUT" ) );
    assert ( x . NewOwner ( "Dejvice", 9873, "CVUT" ) );
    assert ( x . NewOwner ( "Plzen", "Evropska", "Anton Hrabis" ) );
    assert ( x . NewOwner ( "Librec", 4552, "Cvut" ) );

    assert ( x . GetOwner ( "Prague", "Thakurova", owner ) && owner == "CVUT" );
    assert ( x . GetOwner ( "Dejvice", 12345, owner ) && owner == "CVUT" );
    assert ( x . GetOwner ( "Prague", "Evropska", owner ) && owner == "" );
    assert ( x . GetOwner ( "Vokovice", 12345, owner ) && owner == "" );
    assert ( x . GetOwner ( "Prague", "Technicka", owner ) && owner == "CVUT" );
    assert ( x . GetOwner ( "Dejvice", 9873, owner ) && owner == "CVUT" );
    assert ( x . GetOwner ( "Plzen", "Evropska", owner ) && owner == "Anton Hrabis" );
    assert ( x . GetOwner ( "Plzen mesto", 78901, owner ) && owner == "Anton Hrabis" );
    assert ( x . GetOwner ( "Liberec", "Evropska", owner ) && owner == "Cvut" );
    assert ( x . GetOwner ( "Librec", 4552, owner ) && owner == "Cvut" );
    CIterator i3 = x . ListByAddr ();
    assert ( ! i3 . AtEnd ()
             && i3 . City () == "Liberec"
             && i3 . Addr () == "Evropska"
             && i3 . Region () == "Librec"
             && i3 . ID () == 4552
             && i3 . Owner () == "Cvut" );
    i3 . Next ();
    assert ( ! i3 . AtEnd ()
             && i3 . City () == "Plzen"
             && i3 . Addr () == "Evropska"
             && i3 . Region () == "Plzen mesto"
             && i3 . ID () == 78901
             && i3 . Owner () == "Anton Hrabis" );
    i3 . Next ();
    assert ( ! i3 . AtEnd ()
             && i3 . City () == "Prague"
             && i3 . Addr () == "Evropska"
             && i3 . Region () == "Vokovice"
             && i3 . ID () == 12345
             && i3 . Owner () == "" );
    i3 . Next ();
    assert ( ! i3 . AtEnd ()
             && i3 . City () == "Prague"
             && i3 . Addr () == "Technicka"
             && i3 . Region () == "Dejvice"
             && i3 . ID () == 9873
             && i3 . Owner () == "CVUT" );
    i3 . Next ();
    assert ( ! i3 . AtEnd ()
             && i3 . City () == "Prague"
             && i3 . Addr () == "Thakurova"
             && i3 . Region () == "Dejvice"
             && i3 . ID () == 12345
             && i3 . Owner () == "CVUT" );
    i3 . Next ();
    assert ( i3 . AtEnd () );
    assert ( x . Count ( "cvut" ) == 3 );
    CIterator i4 = x . ListByOwner ( "cVuT" );
    assert ( ! i4 . AtEnd ()
             && i4 . City () == "Prague"
             && i4 . Addr () == "Thakurova"
             && i4 . Region () == "Dejvice"
             && i4 . ID () == 12345
             && i4 . Owner () == "CVUT" );
    i4 . Next ();
    assert ( ! i4 . AtEnd ()
             && i4 . City () == "Prague"
             && i4 . Addr () == "Technicka"
             && i4 . Region () == "Dejvice"
             && i4 . ID () == 9873
             && i4 . Owner () == "CVUT" );
    i4 . Next ();
    assert ( ! i4 . AtEnd ()
             && i4 . City () == "Liberec"
             && i4 . Addr () == "Evropska"
             && i4 . Region () == "Librec"
             && i4 . ID () == 4552
             && i4 . Owner () == "Cvut" );
    i4 . Next ();
    assert ( i4 . AtEnd () );
    assert ( x . NewOwner ( "Plzen mesto", 78901, "CVut" ) );
    assert ( x . Count ( "CVUT" ) == 4 );
    CIterator i5 = x . ListByOwner ( "CVUT" );
    assert ( ! i5 . AtEnd ()
             && i5 . City () == "Prague"
             && i5 . Addr () == "Thakurova"
             && i5 . Region () == "Dejvice"
             && i5 . ID () == 12345
             && i5 . Owner () == "CVUT" );
    i5 . Next ();
    assert ( ! i5 . AtEnd ()
             && i5 . City () == "Prague"
             && i5 . Addr () == "Technicka"
             && i5 . Region () == "Dejvice"
             && i5 . ID () == 9873
             && i5 . Owner () == "CVUT" );
    i5 . Next ();
    assert ( ! i5 . AtEnd ()
             && i5 . City () == "Liberec"
             && i5 . Addr () == "Evropska"
             && i5 . Region () == "Librec"
             && i5 . ID () == 4552
             && i5 . Owner () == "Cvut" );
    i5 . Next ();
    assert ( ! i5 . AtEnd ()
             && i5 . City () == "Plzen"
             && i5 . Addr () == "Evropska"
             && i5 . Region () == "Plzen mesto"
             && i5 . ID () == 78901
             && i5 . Owner () == "CVut" );
    i5 . Next ();
    assert ( i5 . AtEnd () );
    assert ( x . Del ( "Liberec", "Evropska" ) );
    assert ( x . Del ( "Plzen mesto", 78901 ) );
    assert ( x . Count ( "cvut" ) == 2 );
    CIterator i6 = x . ListByOwner ( "cVuT" );
    assert ( ! i6 . AtEnd ()
             && i6 . City () == "Prague"
             && i6 . Addr () == "Thakurova"
             && i6 . Region () == "Dejvice"
             && i6 . ID () == 12345
             && i6 . Owner () == "CVUT" );
    i6 . Next ();
    assert ( ! i6 . AtEnd ()
             && i6 . City () == "Prague"
             && i6 . Addr () == "Technicka"
             && i6 . Region () == "Dejvice"
             && i6 . ID () == 9873
             && i6 . Owner () == "CVUT" );
    i6 . Next ();
    assert ( i6 . AtEnd () );

    assert ( x . Add ( "Liberec", "Evropska", "Librec", 4552 ) );
}

static void test1 ( void )
{
    CLandRegister x;
    string owner;

    assert ( x . Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
    assert ( x . Add ( "Prague", "Evropska", "Vokovice", 12345 ) );
    assert ( x . Add ( "Prague", "Technicka", "Dejvice", 9873 ) );
    assert ( ! x . Add ( "Prague", "Technicka", "Hradcany", 7344 ) );
    assert ( ! x . Add ( "Brno", "Bozetechova", "Dejvice", 9873 ) );
    assert ( !x . GetOwner ( "Prague", "THAKUROVA", owner ) );
    assert ( !x . GetOwner ( "Hradcany", 7343, owner ) );
    CIterator i0 = x . ListByAddr ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Evropska"
             && i0 . Region () == "Vokovice"
             && i0 . ID () == 12345
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Technicka"
             && i0 . Region () == "Dejvice"
             && i0 . ID () == 9873
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( ! i0 . AtEnd ()
             && i0 . City () == "Prague"
             && i0 . Addr () == "Thakurova"
             && i0 . Region () == "Dejvice"
             && i0 . ID () == 12345
             && i0 . Owner () == "" );
    i0 . Next ();
    assert ( i0 . AtEnd () );

    assert ( x . NewOwner ( "Prague", "Thakurova", "CVUT" ) );
    assert ( ! x . NewOwner ( "Prague", "technicka", "CVUT" ) );
    assert ( ! x . NewOwner ( "prague", "Technicka", "CVUT" ) );
    assert ( ! x . NewOwner ( "dejvice", 9873, "CVUT" ) );
    assert ( ! x . NewOwner ( "Dejvice", 9973, "CVUT" ) );
    assert ( ! x . NewOwner ( "Dejvice", 12345, "CVUT" ) );
    assert ( x . Count ( "CVUT" ) == 1 );
    CIterator i1 = x . ListByOwner ( "CVUT" );
    assert ( ! i1 . AtEnd ()
             && i1 . City () == "Prague"
             && i1 . Addr () == "Thakurova"
             && i1 . Region () == "Dejvice"
             && i1 . ID () == 12345
             && i1 . Owner () == "CVUT" );
    i1 . Next ();
    assert ( i1 . AtEnd () );

    assert ( ! x . Del ( "Brno", "Technicka" ) );
    assert ( ! x . Del ( "Karlin", 9873 ) );
    assert ( x . Del ( "Prague", "Technicka" ) );
    assert ( ! x . Del ( "Prague", "Technicka" ) );
    assert ( ! x . Del ( "Dejvice", 9873 ) );
}

static void test2 ( void )
{
    CLandRegister x;
    string owner;
    //=====================random test 1=============================================
    assert ( x . Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );

    CIterator it1 = x.ListByAddr();
    assert ( ! it1 . AtEnd ()
             && it1 . City () == "Prague"
             && it1 . Addr () == "Thakurova"
             && it1 . Region () == "Dejvice"
             && it1 . ID () == 12345
             && it1 . Owner () == "" );
    assert ( !it1 . AtEnd () );
    it1 . Next ();
    assert ( it1 . AtEnd () );

    CIterator it2 = x.ListByOwner("");
    assert ( ! it2 . AtEnd ()
             && it2 . City () == "Prague"
             && it2 . Addr () == "Thakurova"
             && it2 . Region () == "Dejvice"
             && it2 . ID () == 12345
             && it2 . Owner () == "" );
    assert ( !it2 . AtEnd () );
    it2 . Next ();
    assert ( it2 . AtEnd () );

    assert ( x . Count ( "" ) == 1 );
    assert ( x . Del ( "Dejvice", 12345 ) );
    assert ( ! (x . Count ( "" ) == 1) );
    //CIterator it3 = x.ListByAddr(); mam tohle osetrit ? kdyz nejsou pozemky
    //assert ( !(it3 . City () == "Prague"
    //         && it3 . Addr () == "Thakurova"
    //         && it3 . Region () == "Dejvice"
    //         && it3 . ID () == 12345
    //         && it3 . Owner () == "") );
    //=========================random test 2================================================

    assert ( x . Add ( "Prague", "Evropska", "Vokovice", 12345 ) );
    assert ( x . Add ( "Prague", "Technicka", "Dejvice", 9873 ) );
    CIterator it4 = x.ListByOwner("");
    assert ( ! it4 . AtEnd ()
             && it4 . City () == "Prague"
             && it4 . Addr () == "Evropska"
             && it4 . Region () == "Vokovice"
             && it4 . ID () == 12345
             && it4 . Owner () == "" );
    it4 . Next();
    assert ( ! it4 . AtEnd ()
             && it4 . City () == "Prague"
             && it4 . Addr () == "Technicka"
             && it4 . Region () == "Dejvice"
             && it4 . ID () == 9873
             && it4 . Owner () == "" );
    it4 . Next();
    assert ( it4 . AtEnd() ) ;

    assert ( x.Count("") == 2);
    assert ( x . NewOwner ( "Vokovice", 12345, "CVUT" ) );
    assert ( x.Count("") == 1);
    assert ( x.Count("cvuT") == 1);
    CIterator it5 = x.ListByOwner("CVut");
    assert ( ! it5 . AtEnd ()
             && it5 . City () == "Prague"
             && it5 . Addr () == "Evropska"
             && it5 . Region () == "Vokovice"
             && it5 . ID () == 12345
             && it5 . Owner () == "CVUT" );
    it5.Next();
    assert ( it5.AtEnd() ) ;
    CIterator it6 = x.ListByOwner("");
    assert ( ! it6 . AtEnd ()
             && it6 . City () == "Prague"
             && it6 . Addr () == "Technicka"
             && it6 . Region () == "Dejvice"
             && it6 . ID () == 9873
             && it6 . Owner () == "" );
    it6.Next();
    assert ( it6.AtEnd() ) ;
    //=========== add test ====================
    assert ( !x . Add ( "Prague", "Evropska", "Progtest", 1345 ) );
    assert ( !x . Add ( "Prog", "Test", "Dejvice", 9873 ) );
    assert ( x . Add ( "Prague", "Cechova", "Bubenec", 9873 ) );
    assert ( x . Add ( "Prague", "Milady", "Bubenec", 999 ) );
    //=============del test============================
    assert ( !x . Del( "Progtest", 1345 ));
    assert ( x . Del ( "Prague", "Cechova"));
    assert ( !x . Del ( "Bubenec ", 999 ));
    //=======================================================================
    assert ( x . GetOwner ( "Bubenec", 999, owner ) && owner == "" );
    //=======================================================================
    //neni pozemek
    assert ( !x . NewOwner ( "Librec", 4552 , "Cvut" ));
    assert ( x . NewOwner ( "Bubenec", 999 , "CVUT" ));
    // stejny vlastnik
    assert ( !x . NewOwner ( "Bubenec", 999 , "CVUT" ));

    assert ( x. Count("PA2") == 0);
    assert ( x. Count("CVUT") == 2 );
    assert ( x. Count("cvut ") != 2 );
    //------------------------------------------------
    assert ( x . Add ( "Brno", "a", "f", 12345 ) );
    assert ( x . Add ( "Brno", "b", "e", 9873 ) );
    assert ( x . Add ( "Brno", "c", "d", 12345 ) );
    assert ( x . Add ( "Brno", "d", "c", 9873 ) );
    assert ( x . Add ( "Brno", "e", "b", 12345 ) );
    assert ( x . Add ( "Brno", "f", "a", 9873 ) );
    assert ( x . Add ( "Ostrava", "a", "Vokovice", 11 ) );
    assert ( x . Add ( "Ostrava", "b", "Dejvice", 22 ) );
    assert ( x . Add ( "Ostrava", "c", "Vokovice", 33 ) );
    assert ( x . Add ( "Ostrava", "d", "Dejvice", 44 ) );
    assert ( x . Add ( "Ostrava", "e", "Vokovice", 55 ) );
    assert ( x . Add ( "Ostrava", "f", "Dejvice", 66 ) );

    assert ( x . NewOwner ( "Brno", "a" , "CVUT" ));
    assert ( x . NewOwner ( "Brno", "b" , "CVUT" ));
    assert ( x . NewOwner ( "Brno", "c" , "CVUT" ));
    assert ( x . NewOwner ( "Brno", "d" , "CVUT" ));
    assert ( x . NewOwner ( "Brno", "e" , "CVUT" ));
    assert ( x . NewOwner ( "Brno", "f" , "CVUT" ));
    assert ( x . NewOwner ( "Vokovice", 11 , "CvUT" ));
    assert ( !x . NewOwner ( "Vokovice ", 33, "cvUT" ));

    assert ( x . NewOwner ( "Vokovice", 33 , "CVUT" ));
    assert ( x . NewOwner ( "Vokovice", 55 , "CVUt" ));
    assert ( x . NewOwner ( "Dejvice", 22 , "CVUT" ));
    assert ( x . NewOwner ( "Dejvice", 44 , "CVUT" ));
    assert ( x . NewOwner ( "Dejvice", 66 , "CVUT" ));

}
int main ( void )
{
    test0();
    test1();
    test2();
    return 0;
}
#endif /* __PROGTEST__ */
