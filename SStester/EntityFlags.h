#pragma once
/*
As component:
pro:
	major:
		  Can order by group -> checking states for part of / whole group should be faster
	minor:
		  More natural / flexible (although EM usage might negate that).
con:
	major:
		  Access with EntityNumber is Enum -> SS -> CDS
	minor:
		  Removes any chance of doubling up entity numbers for ETs which have no shared components.
*/

/*
CONTAINS_SHARED_DATA, this is one way to do it but any deleting would be seriously limited,
a better way perhaps is having specific ETs which are intended for only grabbing data from, EG:

Spider = model,name,stats, pos, animation, speed,ect - no need to copy model/name/stats so this has a Spider with CONTAINS_SHARED_DATA.

to solve:

how do you avoid accessing a Spider that is only meant to get data from? should it be marked inactive?
should there even be Spiders for only getting data from or should they just be marked and used as normal? if this is the case
how do  you delete without large performance hit? (would need to check flags every delete - might not be to bad?)
*/


enum EntityFlags //generic entityFlags, can maybe add in type specific ones later to save space?
{
	ACTIVE = 1,
	CONTAINS_SHARED_DATA = 1 << 10 //if other entities rely on this for data -> need new delete protacol for this situation
};