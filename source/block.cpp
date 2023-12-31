#include "block.hpp"
#include "game.hpp"
#include "util.h"
#include "glext.h"
#include "random.hpp"

// non-generic implementations

GrassBlock::GrassBlock(s16 x, s16 y) : Block(x, y, 14), type(Type::Normal)
{
}

GrassBlock::GrassBlock(s16 x, s16 y, Type type) : Block(x, y, 14), type(type)
{
}

void GrassBlock::draw(const Camera &camera)
{
	pcxImageDraw(&sprGrass, x - camera.x, y - camera.y, GL_FLIP_NONE);

	switch (type)
	{
	case Type::Normal:
		glColor(COLOR_NORMAL);
		break;
	case Type::Spruce:
		glColor(COLOR_SPRUCE);
		break;
	}

	pcxImageDraw(&sprGrassOverlay, x - camera.x, y - camera.y, GL_FLIP_NONE);
	glColor(RGB15(31, 31, 31));
}

u16 GrassBlock::id(void) const
{
	return BID_GRASS;
}

bool GrassBlock::solid(void)
{
	return true;
}

GrassBlock::Type GrassBlock::getType(void)
{
	return type;
}

//-----------------------------------------

Grass::Grass(s16 x, s16 y) : Block(x, y, 1), type(GrassBlock::Type::Normal),
							 height(rng::range(MIN_HEIGHT, MAX_HEIGHT))
{
}

Grass::Grass(s16 x, s16 y, GrassBlock::Type type) : Block(x, y, 1), type(type),
													height(rng::range(MIN_HEIGHT, MAX_HEIGHT))
{
}

void Grass::draw(const Camera &camera)
{
	switch (type)
	{
	case GrassBlock::Type::Normal:
		glColor(GrassBlock::COLOR_NORMAL);
		break;
	case GrassBlock::Type::Spruce:
		glColor(GrassBlock::COLOR_SPRUCE);
		break;
	}

	pcxImageDrawEx(&sprGrass2, x - camera.x, y - camera.y + (16 - height), 0, 0, 16, height, SCALE_NORMAL, GL_FLIP_NONE);
	glColor(RGB15(31, 31, 31));
}

u16 Grass::id(void) const
{
	return BID_GRASS2;
}

bool Grass::solid(void)
{
	return false;
}

GrassBlock::Type Grass::getType(void)
{
	return type;
}

//-----------------------------------------

DirtBlock::DirtBlock(s16 x, s16 y) : Block(x, y, 14), farmland(false), path(false)
{
}

DirtBlock::DirtBlock(s16 x, s16 y, bool farmland, bool path) : Block(x, y, 14), farmland(farmland), path(path)
{
}

void DirtBlock::draw(const Camera &camera)
{
	if (farmland)
		pcxImageDrawEx(&sprDirt, x - camera.x, y - camera.y + 1, 0, 0, 16, 15, SCALE_NORMAL, GL_FLIP_NONE);
	else if (path)
		pcxImageDrawEx(&sprDirtPath, x - camera.y, y - camera.y + 1, 0, 1, 16, 15, SCALE_NORMAL, GL_FLIP_NONE);
	else
		pcxImageDraw(&sprDirt, x - camera.x, y - camera.y, GL_FLIP_NONE);
}

u16 DirtBlock::id(void) const
{
	return BID_DIRT;
}

bool DirtBlock::solid(void)
{
	return true;
}

void DirtBlock::interact(InventoryItem::ID item)
{
	if (!farmland && (item == InventoryItem::ID::WoodenHoe || item == InventoryItem::ID::StoneHoe || item == InventoryItem::ID::IronHoe))
	{
		farmland = true;
		path = false;
	}
	else if (!path && (item == InventoryItem::ID::WoodenShovel || item == InventoryItem::ID::StoneShovel || item == InventoryItem::ID::IronShovel))
	{
		farmland = false;
		path = true;
	}
}

bool DirtBlock::isFarmland(void)
{
	return farmland;
}

bool DirtBlock::isPath(void)
{
	return path;
}

//-----------------------------------------

LeavesBlock::LeavesBlock(s16 x, s16 y, Type type, bool natural) : Block(x, y, 5), natural(natural), type(type)
{
}

void LeavesBlock::draw(const Camera &camera)
{
	switch (type)
	{
	case Type::Oak:
		glColor(COLOR_OAK);
		pcxImageDraw(&sprLeaves, x - camera.x, y - camera.y, GL_FLIP_NONE);
		break;
	case Type::Birch:
		glColor(COLOR_BIRCH);
		pcxImageDraw(&sprBirchLeaves, x - camera.x, y - camera.y, GL_FLIP_NONE);
		break;
	case Type::Spruce:
		glColor(COLOR_SPRUCE);
		pcxImageDraw(&sprSpruceLeaves, x - camera.x, y - camera.y, GL_FLIP_NONE);
		break;
	}
	glColor(RGB15(31, 31, 31)); // reset draw color
}

bool LeavesBlock::solid(void)
{
	return false;
}

u16 LeavesBlock::id(void) const
{
	switch (type)
	{
	case Type::Oak:
	default:
		return BID_LEAVES;
	case Type::Birch:
		return BID_BIRCH_LEAVES;
	case Type::Spruce:
		return BID_SPRUCE_LEAVES;
	}
}

bool LeavesBlock::isNatural(void)
{
	return natural;
}

//-----------------------------------------

FlowerBlock::FlowerBlock(s16 x, s16 y) : Block(x, y, 1), type(FlowerType::Poppy),
										 xOff(rng::rangeSigned(X_OFF_MIN, X_OFF_MAX))
{
	// choose flower type
	switch (rng::range(1, 6))
	{
	case 1:
		type = FlowerType::Poppy;
		break;
	case 2:
		type = FlowerType::Dandelion;
		break;
	case 3:
		type = FlowerType::RedTulip;
		break;
	case 4:
		type = FlowerType::OrangeTulip;
		break;
	case 5:
		type = FlowerType::PinkTulip;
		break;
	case 6:
		type = FlowerType::WhiteTulip;
		break;
	case 7:
		type = FlowerType::Cornflower;
        break;
	}
    // TODO why this no work
}

FlowerBlock::FlowerBlock(s16 x, s16 y, FlowerType type) : Block(x, y, 1), type(type),
														  xOff(rng::rangeSigned(X_OFF_MIN, X_OFF_MAX))
{
}

void FlowerBlock::draw(const Camera &camera)
{
	switch (type)
	{
	case FlowerType::Dandelion:
		pcxImageDraw(&sprDandelion, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
		break;
	case FlowerType::Poppy:
		pcxImageDraw(&sprPoppy, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
		break;
	case FlowerType::RedTulip:
		pcxImageDraw(&sprRedTulip, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
		break;
    case FlowerType::OrangeTulip:
        pcxImageDraw(&sprOrangeTulip, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
        break;
    case FlowerType::PinkTulip:
        pcxImageDraw(&sprPinkTulip, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
        break;
    case FlowerType::WhiteTulip:
        pcxImageDraw(&sprWhiteTulip, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
        break;
    case FlowerType::Cornflower:
        pcxImageDraw(&sprCornflower, x - camera.x + xOff, y - camera.y, GL_FLIP_NONE);
        break;
	}
}

bool FlowerBlock::solid(void)
{
	return false;
}

u16 FlowerBlock::id(void) const
{
	switch (type)
	{
	default:
		return (u16)-1;
	case FlowerType::Poppy:
		return BID_POPPY;
	case FlowerType::Dandelion:
		return BID_DANDELION;
	case FlowerType::RedTulip:
		return BID_RED_TULIP;
    case FlowerType::OrangeTulip:
        return BID_ORANGE_TULIP;
    case FlowerType::PinkTulip:
        return BID_PINK_TULIP;
    case FlowerType::WhiteTulip:
        return BID_WHITE_TULIP;
    case FlowerType::Cornflower:
        return BID_CORNFLOWER;
	}
}

//---------------------------------------------

DoorBlock::DoorBlock(s16 x, s16 y, s16 px, DoorType type) : Block(x, y, 7), open(true), facing(px > x), type(type)
{
}

DoorBlock::DoorBlock(s16 x, s16 y, bool open, bool facing, DoorType type) : Block(x, y, 7), open(open), facing(facing), type(type)
{
}

void DoorBlock::draw(const Camera &camera)
{
	PCXImage *spr;
	switch (type)
	{
	case DoorType::Oak:
	default:
		spr = &sprDoor;
		break;
	case DoorType::Birch:
		spr = &sprBirchDoor;
		break;
	case DoorType::Spruce:
		spr = &sprSpruceDoor;
		break;
	}

	if (open)
		pcxImageDraw(spr, x - camera.x - 1, y - camera.y, GL_FLIP_NONE);
	else
		pcxImageDrawExScaleXY(spr, x - camera.x - 1 + (facing ? 0 : 8), 0, 0, spr->simg.width, spr->simg.height, y - camera.y, 1 << 10, 1 << 12, (facing ? GL_FLIP_NONE : GL_FLIP_H));
}

bool DoorBlock::solid(void)
{
	return !open;
}

void DoorBlock::interact(InventoryItem::ID item)
{
	(void)item;
	if (open)
	{
		open = false;
		playsfx(4, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);
	}
	else
	{
		open = true;
		playsfx(4, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);
	}
}

u16 DoorBlock::id(void) const
{
	switch (type)
	{
	case DoorType::Oak:
	default:
		return BID_DOOR;
	case DoorType::Birch:
		return BID_BIRCH_DOOR;
	case DoorType::Spruce:
		return BID_SPRUCE_DOOR;
	}
}

Rect DoorBlock::getRect(void) const
{
	if (open)
		return Rect(x, y, 16, 32);
	return Rect(x + (facing ? 0 : 11), y, 4, 32);
}

bool DoorBlock::isOpen(void)
{
	return open;
}

bool DoorBlock::getFacing(void)
{
	return facing;
}

//---------------------------------------------

GlassBlock::GlassBlock(s16 x, s16 y) : Block(x, y, 6)
{
}

void GlassBlock::draw(const Camera &camera)
{
	pcxImageDraw(&sprGlass, x - camera.x - 1, y - camera.y, GL_FLIP_NONE);
}

u16 GlassBlock::id(void) const
{
	return BID_GLASS;
}

bool GlassBlock::solid(void)
{
	return true;
}

//---------------------------------------------

static u16 _nextChestID = 0;

ChestBlock::ChestBlock(s16 x, s16 y) : Block(x, y, 7), chid(_nextChestID++)
{
	clear();
}

ChestBlock::ChestBlock(s16 x, s16 y, u16 id) : Block(x, y, 7), chid(id)
{
	_nextChestID = id + 1;
	clear();
}

void ChestBlock::draw(const Camera &camera)
{
	pcxImageDraw(&sprChest, x - camera.x, y - camera.y, GL_FLIP_NONE);
}

bool ChestBlock::solid(void)
{
	return false;
}

u16 ChestBlock::id(void) const
{
	return BID_CHEST;
}

std::array<InventoryItem, ChestBlock::NUM_ITEMS> ChestBlock::getItems(void)
{
	std::array<InventoryItem, NUM_ITEMS> inv;
	for (u8 i = 0; i < NUM_ITEMS; ++i)
		inv[i] = items[i];
	return inv;
}

void ChestBlock::setItem(u8 i, InventoryItem item)
{
	items[i] = item;
}

void ChestBlock::clear(void)
{
	for (int i = 0; i < NUM_ITEMS; ++i)
		items[i] = InventoryItem();
}

u16 ChestBlock::getChestID(void)
{
	return chid;
}

void resetNextChestID(void)
{
	_nextChestID = 0;
}

//----------------------------------------

SignBlock::SignBlock(s16 x, s16 y, const std::string &text) : Block(x, y, 6), text(text), showText(false)
{
}

void SignBlock::draw(const Camera &camera)
{
	pcxImageDraw(&sprSign, x - camera.x, y - camera.y, GL_FLIP_NONE);
}

void SignBlock::drawText(const Camera &camera)
{
	if (showText)
	{
		glBoxFilled(x - camera.x - 2, y - camera.y - 10, x - camera.x - 2 + Game::instance->font.getTextWidth(getText()) + 1, y - camera.y, RGB15(4, 4, 4));
		glBoxStroke(x - camera.x - 2, y - camera.y - 10, Game::instance->font.getTextWidth(getText()) + 2, 11,
					RGB15(11, 11, 11));
		Game::instance->font.print(x - camera.x, y - camera.y - 9, getText(), 0, 0, NULL, SCALE_NORMAL, true);
	}
}

bool SignBlock::solid(void)
{
	return false;
}

u16 SignBlock::id(void) const
{
	return BID_SIGN;
}

const std::string SignBlock::getText(void) const
{
	return text;
}

void SignBlock::setText(const std::string &text)
{
	this->text = text;
}

//----------------------------------------

SlabBlock::SlabBlock(s16 x, s16 y, SlabID slabID, u8 maxBrokenLevel) : Block(x, y, maxBrokenLevel), slabID(slabID)
{
}

bool SlabBlock::solid(void)
{
	return true;
}

bool SlabBlock::isSlab(void)
{
	return true;
}

Rect SlabBlock::getRect(void) const
{
	return Rect(x, y + 8, 16, 8);
}

SlabID SlabBlock::getSlabID(void) const
{
	return slabID;
}

//----------------------------------------

void WheatBlock::setGrowInterval(void)
{
	growInterval = rng::range(GROW_INTERVAL_MIN, GROW_INTERVAL_MAX);
}

WheatBlock::WheatBlock(s16 x, s16 y) : Block(x, y, 1), growStage(0), growInterval(0)
{
	setGrowInterval();
}

WheatBlock::WheatBlock(s16 x, s16 y, u8 growStage) : Block(x, y, 1), growStage(growStage), growInterval(0)
{
	setGrowInterval();
}

void WheatBlock::draw(const Camera &camera)
{
	pcxImageDraw(&sprWheatBlock[growStage], x - camera.x, y - camera.y, GL_FLIP_NONE);
}

u16 WheatBlock::id(void) const
{
	return BID_WHEAT;
}

bool WheatBlock::solid(void)
{
	return false;
}

void WheatBlock::grow(void)
{
	if (--growInterval == 0)
	{
		setGrowInterval(); // randomize grow interval

		// next stage
		if (++growStage > MAX_GROW_STAGE)
			growStage = MAX_GROW_STAGE; // prevent grow being over max grow level
	}
}

u8 WheatBlock::getGrowStage(void)
{
	return growStage;
}

bool WheatBlock::fullyGrown(void)
{
	return growStage >= MAX_GROW_STAGE;
}
