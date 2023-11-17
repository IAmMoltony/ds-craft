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
	 * This is also called when the inventory is constructed.
	 */
	void clear(void);

	/**
	 * @brief Get the number of slots in the inventory
	 */
	u8 getNumSlots(void) const;

	/**
	 * @brief Check if the inventory has the item. Amount is also checked!
	 *
	 * @param item Item to check if the inventory has it
	 * @return true if the item was found; false otherwise
	 */
	bool has(InventoryItem item);

	/**
	 * @brief Add 1 item with the specified ID.
	 * @todo Add a return value to this function
	 *
	 * @param id New item's ID
	 */
	void add(InventoryItem::ID id);

	/**
	 * @brief Add multiple items with the specified ID.
	 * @todo Again, add a return value.
	 *
	 * @param id New item's ID
	 * @param amount How many times to add the item
	 */
	void add(InventoryItem::ID id, u8 amount);

	/**
	 * @brief Check if the inventory is full
	 *
	 * @return true if the inventory is full; false otherwise
	 * @note "Full" = "all slots have a non-None ID and are all max stack"
	 */
	bool full(void);

	/**
	 * @brief Remove one occurence of the item with the given ID
	 *
	 * @param id ID of the item to remove
	 */
	void remove(InventoryItem::ID id);

	/**
	 * @brief Remove multiple occurences of the item with the given ID
	 *
	 * @param id ID of the item to remove
	 * @param amount Maximum amount of occurences to be removed
	 */
	void remove(InventoryItem::ID id, u8 amount);
};