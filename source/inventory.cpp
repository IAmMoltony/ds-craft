#include "inventory.hpp"
#include <algorithm>

Inventory Inventory::itemArrayToInventory(const InventoryItem *array, u8 numSlots)
{
	Inventory inv((u8)numSlots);
	for (u8 i = 0; i < numSlots; ++i)
		inv[i] = array[i];
	return inv;
}

Inventory::Inventory(u8 numSlots) : numSlots(numSlots), items(nullptr), sortMode(true)
{
	items = (InventoryItem *)malloc(sizeof(InventoryItem) * numSlots);
	clear();
}

Inventory::Inventory(const Inventory &other) : Inventory(other.getNumSlots())
{
	for (u8 i = 0; i < numSlots; ++i)
		items[i] = other[i];
}

Inventory::~Inventory()
{
	free(items);
}

InventoryItem Inventory::operator[](u8 i) const
{
	if (i >= numSlots)
		return InventoryItem(); // fallback in case request out of bounds item
	return items[i];
}

InventoryItem &Inventory::operator[](u8 i)
{
	return items[i];
}

void Inventory::clear(void)
{
	for (u8 i = 0; i < numSlots; ++i)
		items[i] = InventoryItem();
}

void Inventory::sort(void)
{
	std::sort(items, &items[numSlots], sortMode ? InventoryItem::compareByID : InventoryItem::compareByAmount);
	std::reverse(items, &items[numSlots]);
	sortMode = !sortMode;
}

u8 Inventory::getNumSlots(void) const
{
	return numSlots;
}

bool Inventory::has(InventoryItem item)
{
	// handling any planks
	if (item.id == InventoryItem::ID::AnyPlanks)
	{
		// check every planks item
		for (int i = 0; i < InventoryItem::numPlanksItemIDs; ++i)
		{
			if (has(InventoryItem(InventoryItem::planksItemIDs[i], item.amount)))
				return true;
		}
		return false;
	}

	for (u8 i = 0; i < numSlots; ++i)
	{
		// if the amount of the item is >= desired amount and the id is the same
		// then we have item
		InventoryItem checkItem = items[i];
		if (checkItem.id == item.id && checkItem.amount >= item.amount)
			return true;
	}
	return false;
}

void Inventory::add(InventoryItem::ID id)
{
	// check if inventory is full
	if (full())
		return;

	u8 maxStack = InventoryItem(id, 1).getMaxStack();

	// If item is stackable, try to find its stack
	if (maxStack > 1)
	{
		for (u8 i = 0; i < numSlots; ++i)
		{
			// stack is full; skip!
			if (items[i].amount >= maxStack)
				continue;

			// found stack
			if (items[i].id == id)
			{
				++items[i].amount;
				return;
			}
		}
	}

	// item is either not stackable or we didn't find a stack
	// in this case let's make a new stack
	for (u8 i = 0; i < numSlots; ++i)
	{
		if (items[i].id == InventoryItem::ID::None)
		{
			// empty slot, occupy it
			items[i].id = id;
			++items[i].amount;
			return;
		}
	}
}

void Inventory::add(InventoryItem::ID id, u8 amount)
{
	// all this function does it just executes the overload
	// without amount multiple times
	// this might be quite inefficient, but eh
	// doesn't seem to cause any issues rn

	for (u8 _ = 0; _ < amount; ++_)
		add(id);
}

void Inventory::add(InventoryItem item)
{
	add(item.id, item.amount);
}

bool Inventory::full(void)
{
	for (u8 i = 0; i < numSlots; ++i)
	{
		InventoryItem item = items[i];
		u8 amount = item.amount;
		u8 maxStack = item.getMaxStack();
		if (amount < maxStack || item.id == InventoryItem::ID::None)
			return false; // Inventory is not full: one of the items is not max stacked or it's empty space
	}
	return true;
}

void Inventory::remove(InventoryItem::ID id)
{
	if (id == InventoryItem::ID::AnyPlanks)
	{
		// special case: any planks
		// for every single planks item ID, we check if we have it
		// if we do, then we remove it and move on with our life
		// if all cases fail, then we just act like we don't care
		// and return
		for (int i = 0; i < InventoryItem::numPlanksItemIDs; ++i)
		{
			InventoryItem::ID piid = InventoryItem::planksItemIDs[i]; // Short for "Planks Item ID"
			InventoryItem planksItem(piid, 1);
			if (has(planksItem))
			{
				remove(piid);
				return;
			}
		}
		return;
	}

	for (u8 i = 0; i < 20; ++i)
	{
		// cheeck if the item exists and it has the desired ID
		if (items[i].id == id && items[i].amount > 0)
		{
			--items[i].amount; // remove one of the i8tem

			// if there are no more items in the slot,
			// set its ID to none.
			if (items[i].amount == 0)
				items[i].id = InventoryItem::ID::None;
			return;
		}
	}
}

void Inventory::remove(InventoryItem::ID id, u8 amount)
{
	for (u8 _ = 0; _ < amount; ++_)
		remove(id);
}

void Inventory::remove(InventoryItem item)
{
	remove(item.id, item.amount);
}
