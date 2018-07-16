﻿/*	Class:		WordResultType
 *	Purpose:	To return word variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2018r2 (Natural Intelligence)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at http://mafait.org/contact/
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/

#include "Item.h"

class WordResultType
	{
	friend class AdminItem;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class Item;
	friend class List;
	friend class WordItem;
	friend class WordList;
	friend class WordTypeList;

	protected:
	// Protected variables

	signed char result;

	WordItem *createdWordItem;
	WordItem *foundWordItem;

	WordTypeItem *foundWordTypeItem;

	protected:
	// Constructor

	WordResultType()
		{
		result = RESULT_OK;

		createdWordItem = NULL;
		foundWordItem = NULL;

		foundWordTypeItem = NULL;
		}
	};

/*************************************************************************
 *	"I take joy in doing your will, my God,
 *	for your instructions are written on my heart." (Psalm 40:8)
 *************************************************************************/
