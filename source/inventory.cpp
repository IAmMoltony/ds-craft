#include "inventory.hpp"

Inventory::Inventory(u8 numSlots) : numSlots(numSlots), items(nullptr)
{
	items = (InventoryItem *)malloc(sizeof(InventoryItem) * numSlots);
	clear();
}

Inventory::~Inventory()
{
	free(items);
}

InventoryItem Inventory::operator[](u8 i) const
{
	if (i >= numSlots)
		return InventoryItem();
	return items[i];
}

InventoryItem &Inventory::operator[](u8 i)
{
	if (i >= numSlots)
		return InventoryItem();
	return items[i];
}

void Inventory::clear(void)
{
	for (u8 i = 0; i < numSlots; ++i)
		items[i] = InventoryItem();
}

u8 Inventory::getNumSlots(void)
{
	return numSlots;
}