/**
 * @file inventory.hpp
 * @brief Inventory class
 */

#pragma once

#include "item.hpp"

/**
 * @brief Inventory class
 *
 * Inventory contains items.
 */
class Inventory
{
private:
	/**
	 * @brief How many slots the inventory has
	 */
	u8 numSlots;

	/**
	 * @brief Array of inventory items
	 */
	InventoryItem *items;

public:
	/**
	 * @brief Construct an inventory
	 * @param numSlots number of slots (items) in the inventory
	 */
	Inventory(u8 numSlots);

	// Deleted because copying and setting inventories is not used anywhere.
	Inventory(const Inventory &) = delete;
	Inventory &operator=(const Inventory &) = delete;

	/**
	 * @brief Destroy inventory
	 */
	~Inventory();

	/**
	 * @brief Get an item from the inventory
	 * @param i item index (0 to num slots - 1)
	 */
	InventoryItem operator[](u8 i) const;

	/**
	 * @brief Set an item in the inventory
	 * @param i item index (0 to num slots - 1)
	 */
	InventoryItem &operator[](u8 i);

	/**
	 * @brief Clear the inventory
	 *
	 * This is called in the constructor as well.
	 */
	void clear(void);

	/**
	 * @brief Get the number of slots in the inventory
	 */
	u8 getNumSlots(void);
};