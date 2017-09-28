/*	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		To store collection items
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

#include "CollectionItem.cpp"
#include "List.h"

class CollectionList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsCollectionWord_;


	// Private functions

	void deleteCollectionList( CollectionItem *searchCollectionItem )
		{
		CollectionItem *deleteCollectionItem;

		while( searchCollectionItem != NULL )
			{
			deleteCollectionItem = searchCollectionItem;
			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			delete deleteCollectionItem;
			}
		}

	CollectionItem *firstActiveCollectionItem()
		{
		return (CollectionItem *)firstActiveItem();
		}

	protected:
	// Constructor

	CollectionList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		isMarkedAsCollectionWord_ = false;

		initializeListVariables( WORD_COLLECTION_LIST_SYMBOL, "CollectionList", commonVariables, inputOutput, myWordItem );
		}

	~CollectionList()
		{
		deleteCollectionList( firstActiveCollectionItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has replaced items." );

		deleteCollectionList( (CollectionItem *)firstDeletedItem() );
		}


	// Protected functions

	void addToCollectionWordQuickAccessList()
		{
		WordItem *lastCollectionWordItem = commonVariables()->lastCollectionWordItem;

		if( lastCollectionWordItem == NULL )
			commonVariables()->firstCollectionWordItem = myWordItem();
		else
			// Word order is important: Add word at end of collection word list
			lastCollectionWordItem->nextCollectionWordItem = myWordItem();

		commonVariables()->lastCollectionWordItem = myWordItem();
		}

	bool hasCollectionNr( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->commonWordItem() == commonWordItem )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasCollectionNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->collectionWordItem() == collectionWordItem &&
				searchCollectionItem->commonWordItem() == commonWordItem )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasNonExclusiveCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( !searchCollectionItem->isExclusiveSpecification() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isCollectionSpanishAmbiguous( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->collectionWordItem() == searchCollectionItem->commonWordItem() )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isNonCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( !searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	unsigned short highestCollectionOrderNr( unsigned int collectionNr )
		{
		unsigned short highestCollectionOrderNr = NO_ORDER_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->collectionOrderNr() > highestCollectionOrderNr )
					highestCollectionOrderNr = searchCollectionItem->collectionOrderNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return highestCollectionOrderNr;
		}

	unsigned short collectionOrderNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->collectionNr() == collectionNr &&
			searchCollectionItem->collectionWordItem() == collectionWordItem &&
			searchCollectionItem->commonWordItem() == commonWordItem )
				return searchCollectionItem->collectionOrderNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NO_ORDER_NR;
		}

	unsigned int collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > NO_WORD_TYPE_NR &&
		commonWordItem != NULL )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->commonWordItem() == commonWordItem &&
				searchCollectionItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchCollectionItem->collectionNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int compoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		unsigned int foundCompoundCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > NO_WORD_TYPE_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					foundCompoundCollectionNr = searchCollectionItem->collectionNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return foundCompoundCollectionNr;
		}

	unsigned int nonCompoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > NO_WORD_TYPE_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( !searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchCollectionItem->collectionNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int nonCompoundCollectionNr( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		WordItem *collectionWordItem;

		if( compoundCollectionNr > NO_CONTEXT_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == compoundCollectionNr &&
				( collectionWordItem = searchCollectionItem->collectionWordItem() ) != NULL )
					{
					if( ( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr( searchCollectionItem->collectionWordTypeNr() ) ) > NO_COLLECTION_NR )
						return nonCompoundCollectionNr;
					}

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int collectionNrByCompoundGeneralizationWord( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != NULL )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isExclusiveSpecification() == isExclusiveSpecification &&
				searchCollectionItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
				searchCollectionItem->compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
					return searchCollectionItem->collectionNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int highestCollectionNr()
		{
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->collectionNr() > highestCollectionNr )
				highestCollectionNr = searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return highestCollectionNr;
		}

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->collectionWordItem() == unusedWordItem )
				return startError( functionNameString, "The collected word item is still in use" );

			if( searchCollectionItem->commonWordItem() == unusedWordItem )
				return startError( functionNameString, "The common word item is still in use" );

			if( searchCollectionItem->compoundGeneralizationWordItem() == unusedWordItem )
				return startError( functionNameString, "The compound word item is still in use" );

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return RESULT_OK;
		}

	WordItem *collectionWordItem( unsigned int compoundCollectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		// notThisCommonWordItem might be undefined

		if( compoundCollectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == compoundCollectionNr )
					return searchCollectionItem->collectionWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *commonWordItem( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr )
					return searchCollectionItem->commonWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( compoundCollectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == compoundCollectionNr )
					return searchCollectionItem->compoundGeneralizationWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *feminineCollectionWordItem()
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->hasFemaleCollectionWord() )
				return searchCollectionItem->collectionWordItem();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NULL;
		}

	WordItem *masculineCollectionWordItem()
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->hasMaleCollectionWord() )
				return searchCollectionItem->collectionWordItem();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NULL;
		}

	BoolResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		bool isCollected = false;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findCollection";

		if( commonWordItem == NULL )
			return startBoolResultError( functionNameString, "The given collected word is undefined" );

		if( collectionWordItem == NULL )
			return startBoolResultError( functionNameString, "The given common word is undefined" );

		if( ( searchCollectionItem = firstActiveCollectionItem() ) != NULL )
			{
			do	{
				if( searchCollectionItem->collectionWordItem() == collectionWordItem &&

				( searchCollectionItem->commonWordItem() == commonWordItem ||

				( isAllowingDifferentCommonWord &&
				collectionWordItem->hasCollectionNr( searchCollectionItem->collectionNr() ) ) ) )
					isCollected = true;
				}
			while( !isCollected &&
			( searchCollectionItem = searchCollectionItem->nextCollectionItem() ) != NULL );
			}

		boolResult.booleanValue = isCollected;
		return boolResult;
		}

	CollectionResultType addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int _collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		bool isDuplicateCollection = false;
		unsigned short collectionOrderNr;
		unsigned int foundCollectionNr = NO_COLLECTION_NR;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollection";

		if( collectionWordTypeNr <= NO_WORD_TYPE_NR ||
		collectionWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startCollectionResultError( functionNameString, "The given collected word type number is undefined or out of bounds: ", collectionWordTypeNr );

		if( collectionWordItem == NULL )
			return startCollectionResultError( functionNameString, "The given collected word is undefined" );

		if( commonWordItem == NULL )
			return startCollectionResultError( functionNameString, "The given common word is undefined" );

		if( collectionWordItem == myWordItem() )
			return startCollectionResultError( functionNameString, "The given collected word is the same as my word" );

		if( collectionWordTypeNr == WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = WORD_TYPE_NOUN_SINGULAR;

		// Typically for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !myWordItem()->hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( functionNameString, "I don't have the requested word type number: ", collectionWordTypeNr );

		// Typically for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !collectionWordItem->hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( functionNameString, "The collection word item doesn't have the requested word type number: ", collectionWordTypeNr );

		if( _collectionNr == NO_COLLECTION_NR )
			{
			if( ( _collectionNr = myWordItem()->highestCollectionNrInCollectionWords() ) >= MAX_COLLECTION_NR )
				return startCollectionResultSystemError( functionNameString, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++_collectionNr;
			}

		foundCollectionNr = collectionNr( collectionWordTypeNr, commonWordItem );

		if( isSpecificationGeneralization &&
		foundCollectionNr > NO_COLLECTION_NR &&
		foundCollectionNr != _collectionNr )
			isDuplicateCollection = true;

		if( !isDuplicateCollection &&

		( collectionResult.createdCollectionNr > NO_COLLECTION_NR ||
		// Check if collection already exists
		!myWordItem()->hasCollectionNr( _collectionNr, collectionWordItem, commonWordItem ) ) )
			{
			// A collection comes in pairs
			if( ( collectionOrderNr = myWordItem()->highestCollectionOrderNrInCollectionWords( _collectionNr ) ) >= MAX_ORDER_NR - 1 )
				return startCollectionResultSystemError( functionNameString, "Collection order number overflow" );

			if( !isMarkedAsCollectionWord_ )
				{
				isMarkedAsCollectionWord_ = true;
				addToCollectionWordQuickAccessList();
				}

			if( addItemToList( QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, _collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, commonVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
				return addCollectionResultError( functionNameString, "I failed to add an active collection item" );
			}

		return collectionResult;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
