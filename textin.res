        ��  ��                  n  <   R T _ R C D A T A   T E X T R E S       0         CELL	NAM0		Not sure. Appears before a list of object references in a cell.?
CELL	MPDT	float	Map Data: x,y,z floats) [ESS]
CELL	MPNT		Map Note, String) [ESS]
CELL	MVRF		Move Reference) [ESS,ESP] Signifies that the reference has been moved to another cell. If moved to an interior cell is followed by CNAM. (And other times? I forget.)Found frequently in ESS files as creatures and NPCS move (or are moved) from their original.Can also be found (very rarely) in an esp if the esp moves a reference from a master file.
CELL	NAM8		Visibility Map) [ESS]
CELL	NAM0	int	����� �������� � ������. Normal Objects Count, Long) [ESM,ESP] Corresponds to __TEMP_REFS___ line Count of non "reference persists" objects in cell  / Appears between "persistent" and non-"persistent" objects
CELL	ZNAM		Disabled: 1 byte) [ESS?]  Probably presence is enough.  Byte always == 0?
CELL	NAM5	byte	����� RGBN
CELL	DATA	float	24�����) ���������� xyz rxryrz
CELL	MVRF	int	Moved ref ��� ����������, ����� load ref/ . +����� �����������
CELL	CNDT	int	���������� ������
CELL	XSCL	float	Scale
CELL	FRMR	FRMR	index of new object and esm index
CELL	INTV	int	Its Extra Data checkbox - INT field. no otricat.
CELL	NAM9	int	Its Extra Data checkbox - Object count? for Gold_001 mb.
CELL	ANAM	string	Its Extra Data checkbox - Owner.
CELL	BNAM	string	Its Extra Data checkbox - Varable.
CELL	UNAM	byte	Reference Blocked checkbox is check.
CELL	XCHG	int	Its Extra Data checkbox - Enchant Charge. Secret field.
CELL	CNAM	string	Its Extra Data checkbox - Faction.
CELL	INDX	int	Its Extra Data checkbox - Faction Rank.
CLAS	CLDT	int	atr1 atr2 spec 10skill Flags autoc
CNTC	NPCO	int	�����[int] + ��������
CONT	NPCO	int	Inventory of a creature or NPC; contents of a container.
CREA	NPDT	byte	nformation is a set of 1-byte integers, listed in this order:
CREA	FLAG	byte	This is a bitmask, containing values for several fields on the monster^ 
CREA	XSCL	float	The scale of the object as a floating point number from 0.0 to 1.0.
CREC	NPCO	int	�����[int] + ��������
CREC	INDX	int	ID index...
DOOR	FLTV		Additional information for a reference to an object. Used to hold lock information for chests and doors.
DOOR	KNAM	string	Name of an associated object. E.g., could be the name of the key that unlocks this chest or door.
FACT	FADT		[Not decoded yet.] However, this looks like a list of the attribute and skill levels required to attain each of the faction ranks, as well as the general disposition of other faction members to someone with that rank. 
GAME	GMDT	string	
INFO	ACDT	string	who
INGR	IRDT	int	
INGR	ITEX	string	
NPC_	NPCO	int	Inventory of a creature or NPC; contents of a container.That is an uppercase oh not a zero. This is a repeating field, one for each thing carried in the NPC's inventory."NPCO" -- That is an uppercase oh not a zero. This is a repeating field, one for each thing carried in the NPC's inventory.   4May vary, but appears to typically (always?) be 0x24, with extra unused space at the end of the NPCO record padded using nulls.Indicates how many of that item are carried.   4This the unique name/ID of the item being carried.
NPC_	NPDT	byte	nformation is a set of 1-byte integers, listed in this order:
NPC_	FLAG	byte	This is a bitmask, containing values for several fields on the NPC. 
NPC_	NPCS		This is a repeating field, and appears once for each spell, ability, or power the npc has.
NPC_	AIDT	byte	12 byte: 01hello 2fight 3flee 4alarm 567unk 89 10 11Servise
NPC_	AI_W	byte	14byte AIWander: 01distance 23duration 456789 10 11 12 13more 
NPCC	NPCO	int	Inventory of a creature or NPC; contents of a container.That is an uppercase oh not a zero. This is a repeating field, one for each thing carried in the NPC's inventory."NPCO" -- That is an uppercase oh not a zero. This is a repeating field, one for each thing carried in the NPC's inventory.   4May vary, but appears to typically (always?) be 0x24, with extra unused space at the end of the NPCO record padded using nulls.Indicates how many of that item are carried.   4This the unique name/ID of the item being carried.
NPCC	NPDT	byte	51bytesNoautocalc or 12autocalc o Bytes 0-1: Base disposition Bytes 2-3: Flag? values: {0,2,4,8,10,14}Bytes 4-8: Index number
NPCC	FLAG	byte	This is a bitmask, containing values for several fields on the NPC. 
NPCC	NPCS		This is a repeating field, and appears once for each spell, ability, or power the npc has.
NPCC	AIDT	byte	12 byte: 01hello 2fight 3flee 4alarm 567unk 89 10 11Servise
NPCC	AI_W	byte	14byte AIWander: 01distance 23duration 456789 10 11 12 13more 
NPCC	SLCS 		script variable�?
NPCC	WIDX		Wear index: index of items within the list that are worn) But why 8 bytes???
NPCC	XCHG		Charge? float)
NPCC	XIDX		Subindex of NPCO, precedes greater detail)
NPCC	XHLT		Health)Format (integer or float) depends on type of item.
NPCC	XSOL		Soul gem contained creature, zero terminated string creature id)
SCPT	SCVR		Each variable name is listed, separated by a null.
SCPT	SCDT		Compiled version of the script.
SCPT	SLFD	float	Float Data: 4 bytes x num of floats)
SCPT	SLSD	short	Short Data 2 bytes x num of shorts)
SCPT	RNAM		Reference)Reference object for global script. Same format as CELL.FRMR.
SPLM	NPDT	string	string[20?] + ��� � ����� 2 ���, ����� ��� float, ����� ��� ���?
SPLM	NAME	int	
SPLM	NAM0	byte	
TES3	GMDT	float	6float; text ��� �����; � ��� +#��� �����
TES3	SCRS		������ 4� �������� ��
WEAP	ITEX	string	Inventory pic for items  