/*	Class:		UserSpecificationResultType
 *	Purpose:	To return user specification variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2017r2 (Science as it should be)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include "WordItem.h"

class UserSpecificationResultType
	{
	friend class Item;
	friend class WordSpecification;

	protected:
	// Protected variables

	signed char result;

	bool isConfirmedExclusive;
	bool isConfirmedSpecificationButNotItsRelation;
	bool isNonExclusiveSpecification;
	bool isSameQuestionFromUser;
	bool isSimilarOrRelatedQuestion;

	SpecificationItem *confirmedSpecificationItem;

	protected:
	// Constructor

	UserSpecificationResultType()
		{
		result = RESULT_OK;

		isConfirmedExclusive = false;
		isConfirmedSpecificationButNotItsRelation = false;
		isNonExclusiveSpecification = false;
		isSameQuestionFromUser = false;
		isSimilarOrRelatedQuestion = false;

		confirmedSpecificationItem = NULL;
		}
	};

/*************************************************************************
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *************************************************************************/
