# SStester---Copy
This is an entity component system made using sparse set, where entity types can inherit from eachother to make some searching paterns easy.

requires c++20,

For Mike:

Disregard files EntityFlags, SparseSet, EntityType, EntityTester

Only important parts of PerfectET are ET_ID,Comp_ID, generic template of ETInfo<id> (line 333), ET<id> (line 798), Comp<id> (line 994)


EntityNum32Bit just a uint32_t with some functions attached (this is in object.h)


Entity<id> just a EntityNum32Bit but templating it can be helpful in some instances


TwoSortsSparse<Comp_ID> funily enough only really makes sense to sort by entity type (generally) as i found out after naming it,
so can ignore sorting in this class. basics of this class are:

-mSprase[entityNumber] gives index for component data that are in mCDS, mEDS[index] gives entityNumber for data that are in mCDS,
  
-mCDS/mEDS are sorted by entity type
  
-Bounds are the start and end index of a given entity type,
  
-the end goal is to get use bounds to iterate through mCDS as it is cache friendly.


ETData<ET_ID id> just a class that provides tuple consisting of the types of the components in ET<id>, to avoid function to create Entity
requiring long list of inputs when ET<id> is large.


Program works by having an EntityManager that stores sparse sets of all Comp<id> and acts as an interface to allow adding and deleting of entities and provides
tools to access data of entities in different ways

TestSystem runs through the basic operations of EntityManger, isn't very extensive as i ran into a problem on thursday where i realized c++ has 
no way to iterate through tuples natively, and the obvious solutions don't really work for most of what i need. (it is apparently coming in 2023, somewhat fustratingly)
