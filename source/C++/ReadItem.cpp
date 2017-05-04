/*	Class:			ReadItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about the read words of a sentence
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
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

#ifndef READITEM
#define READITEM 1
#include "GrammarItem.cpp"

class ReadItem : private Item
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminReadSentence;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class ReadList;

	// Private initialized variables

	unsigned short wordOrderNr_;
	unsigned short wordParameter_;
	unsigned short wordTypeNr_;

	WordItem *readWordItem_;


	protected:
	// Protected constructed variables

	bool hasWordPassedIntegrityCheckOfStoredUserSentence;
	bool isMarkedBySetGrammarParameter;

	unsigned short grammarParameter;

	GrammarItem *definitionGrammarItem;


	// Protected initialized variables

	char *readString;


	// Constructor

	ReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *_readString, WordItem *readWordItem, CommonVariables *commonVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ReadItem", commonVariables, inputOutput, myList, myWordItem );


		// Private initialized variables

		wordOrderNr_ = wordOrderNr;
		wordParameter_ = wordParameter;
		wordTypeNr_ = wordTypeNr;

		readWordItem_ = readWordItem;


		// Protected constructed variables

		hasWordPassedIntegrityCheckOfStoredUserSentence = false;
		isMarkedBySetGrammarParameter = false;

		grammarParameter = NO_GRAMMAR_PARAMETER;

		definitionGrammarItem = NULL;

		// Protected initialized variables

		readString = NULL;

		if( _readString != NULL )
			{
			if( readStringLength < MAX_SENTENCE_STRING_LENGTH &&
			strlen( _readString ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( readString = new char[readStringLength + 1] ) != NULL )
					{
					strcpy( readString, EMPTY_STRING );
					strncat( readString, _readString, readStringLength );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the read string" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read string is too long" );
			}
		}

	~ReadItem()
		{
		if( readString != NULL )
			delete readString;
		}


	// Protected virtual functions

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( readString != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, readString );
			}
		}

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;

		statusString[0] = statusChar();

		if( readWordItem_ != NULL &&
		( wordString = readWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( grammarParameter == queryParameter ||
				wordOrderNr_ == queryParameter ||
				wordParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( grammarParameter > NO_GRAMMAR_PARAMETER ||
				wordOrderNr_ > NO_ORDER_NR ||
				wordParameter_ > NO_WORD_PARAMETER ) ) );
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( readWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : readWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : readWordItem_->itemNr() == queryItemNr ) ) ||

				( definitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : definitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : definitionGrammarItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		ReadItem *nextSortReadItem = (ReadItem *)nextSortItem;

		// Remark: All read items should have the same creationSentenceNr
		return ( nextSortItem != NULL &&

				// 1) Ascending wordOrderNr_
				( wordOrderNr_ < nextSortReadItem->wordOrderNr_ ||

				// 2) Descending wordTypeNr_
				( wordOrderNr_ == nextSortReadItem->wordOrderNr_ &&
				wordTypeNr_ > nextSortReadItem->wordTypeNr_ ) ) );
		}

	virtual char *itemString()
		{
		return readString;
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *wordString;
		char *wordTypeString = myWordItem()->wordTypeNameString( wordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( hasWordPassedIntegrityCheckOfStoredUserSentence )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "hasWordPassedIntegrityCheckOfStoredUserSentence" );
			}

		if( isMarkedBySetGrammarParameter )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isMarkedBySetGrammarParameter" );
			}

		if( wordOrderNr_ > NO_ORDER_NR )
			{
			sprintf( tempString, "%cwordOrderNr:%u", QUERY_SEPARATOR_CHAR, wordOrderNr_ );
			strcat( queryString, tempString );
			}

		if( wordParameter_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%cwordParameter:%u", QUERY_SEPARATOR_CHAR, wordParameter_ );
			strcat( queryString, tempString );
			}

		if( grammarParameter > NO_GRAMMAR_PARAMETER )
			{
			sprintf( tempString, "%cgrammarParameter:%u", QUERY_SEPARATOR_CHAR, grammarParameter );
			strcat( queryString, tempString );
			}

		if( readString != NULL )
			{
			sprintf( tempString, "%creadString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, readString, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( wordTypeString == NULL )
			sprintf( tempString, "%cwordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );
		else
			sprintf( tempString, "%cwordType:%s%c%u", QUERY_SEPARATOR_CHAR, wordTypeString, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );

		strcat( queryString, tempString );

		if( readWordItem_ != NULL )
			{
			sprintf( tempString, "%creadWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, readWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, readWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = readWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( definitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cdefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, definitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, definitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}

	virtual BoolResultType findMatchingWordReferenceString( char *queryString )
		{
		BoolResultType boolResult;

		if( readWordItem_ != NULL )
			return readWordItem_->findMatchingWordReferenceString( queryString );

		return boolResult;
		}


	// Protected functions

	bool hasFoundRelationWordInThisList( WordItem *relationWordItem )
		{
		ReadItem *searchReadItem = this;

		if( relationWordItem != NULL )
			{
			while( searchReadItem != NULL )
				{
				if( searchReadItem->isRelationWord() &&
				searchReadItem->readWordItem() == relationWordItem )
					return true;

				searchReadItem = searchReadItem->nextReadItem();
				}
			}

		return false;
		}

	bool isSymbol()
		{
		return ( wordTypeNr_ == WORD_TYPE_SYMBOL );
		}

	bool isNumeral()
		{
		return ( wordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isAdjective()
		{
		return ( wordTypeNr_ == WORD_TYPE_ADJECTIVE );
		}

	bool isArticle()
		{
		return ( wordTypeNr_ == WORD_TYPE_ARTICLE );
		}

	bool isNoun()
		{
		return isNounWordType( wordTypeNr_ );
		}

	bool isSingularNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isMatchingReadWordTypeNr( unsigned short wordTypeNr )
		{
		return isMatchingWordType( wordTypeNr_, wordTypeNr );
		}

	bool isDeterminerOrPronoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE ||
				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE ||

				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ||

				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE ||
				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE ||

				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL );
		}

	bool isPossessiveDeterminer()
		{
		return ( wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL );
		}

	bool isPreposition()
		{
		return ( wordTypeNr_ == WORD_TYPE_PREPOSITION );
		}

	bool isProperName()
		{
		return ( wordTypeNr_ == WORD_TYPE_PROPER_NAME );
		}

	bool isVerb()
		{
		return ( wordTypeNr_ == WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_VERB_PLURAL );
		}

	bool isPluralQuestionVerb()
		{
		return ( wordTypeNr_ == WORD_TYPE_VERB_PLURAL &&
				grammarParameter == GRAMMAR_QUESTION_VERB );
		}

	bool isText()
		{
		return ( wordTypeNr_ == WORD_TYPE_TEXT );
		}

	bool isNumeralBoth()
		{
		return ( wordParameter_ == WORD_PARAMETER_NUMERAL_BOTH );
		}

	bool isAdjectiveAssigned()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	bool isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	bool isAdjectiveCalledSingularFeminineOrMasculine()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE );
		}

	bool isAdjectiveEvery()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE );
		}

	bool isAdjectivePrevious()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE );
		}

	bool isNegative()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_NO ||
				wordParameter_ == WORD_PARAMETER_ADVERB_NOT ||
				wordParameter_ == WORD_PARAMETER_ADVERB_NOT_FRENCH );
		}

	bool isNounJustificationReport()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT );
		}

	bool isNounInformation()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_INFORMATION );
		}

	bool isNounValue()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

	bool isNounFile()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_FILE ||
				wordParameter_ == WORD_PARAMETER_NOUN_TEST_FILE );
		}

	bool isNounPartOf()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_PART );
		}

	bool isSeparator()
		{
		return ( wordParameter_ == WORD_PARAMETER_SYMBOL_COMMA ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_COLON ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_QUESTION_MARK ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED );
		}

	bool isVirtualListPreposition()
		{
		return ( wordParameter_ == WORD_PARAMETER_PREPOSITION_FROM ||
				wordParameter_ == WORD_PARAMETER_PREPOSITION_TO ||
				wordParameter_ == WORD_PARAMETER_PREPOSITION_OF );
		}

	bool isFrenchPreposition()
		{
		return ( wordParameter_ == WORD_PARAMETER_PREPOSITION_FRENCH_A );
		}

	bool isUserDefined()
		{
		return ( wordParameter_ == NO_WORD_PARAMETER );
		}

	bool isGeneralizationWord()
		{
		return ( grammarParameter == GRAMMAR_GENERALIZATION_WORD );
		}

	bool isSpecificationWord()
		{
		return ( grammarParameter == GRAMMAR_SPECIFICATION_WORD );
		}

	bool isRelationWord()
		{
		// To avoid triggering on the article before a proper name preceded by defined article
		return ( wordTypeNr_ != WORD_TYPE_ARTICLE &&
				grammarParameter == GRAMMAR_RELATION_WORD );
		}

	bool isLinkedGeneralizationConjunction()
		{
		return ( grammarParameter == GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION );
		}

	bool isSentenceConjunction()
		{
		return ( grammarParameter == GRAMMAR_SENTENCE_CONJUNCTION );
		}

	unsigned short wordOrderNr()
		{
		return wordOrderNr_;
		}

	unsigned short wordParameter()
		{
		return wordParameter_;
		}

	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	signed char changeReadWord( unsigned short newWordTypeNr, WordItem *newReadWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeReadWord";

		if( newReadWordItem == NULL )
			return startError( functionNameString, NULL, "The given new read word item is undefined" );

		wordTypeNr_ = newWordTypeNr;
		readWordItem_ = newReadWordItem;

		return RESULT_OK;
		}

	char *readWordTypeString()
		{
		if( readWordItem_ != NULL )
			return readWordItem_->activeWordTypeString( wordTypeNr_ );

		return NULL;
		}

	ReadItem *firstRelationWordReadItem()
		{
		ReadItem *searchReadItem = this;

		while( searchReadItem != NULL )
			{
			if( searchReadItem->isRelationWord() )
				return searchReadItem;

			searchReadItem = searchReadItem->nextReadItem();
			}

		return NULL;
		}

	ReadItem *nextReadItem()
		{
		return (ReadItem *)nextItem;
		}

	WordItem *readWordItem()
		{
		return readWordItem_;
		}

	WordTypeItem *activeReadWordTypeItem()
		{
		return ( readWordItem_ == NULL ? NULL : readWordItem_->activeWordTypeItem( true, wordTypeNr_ ) );
		}
	};
#endif

/*************************************************************************
 *	"The godly will see these things and be glad,
 *	while the wicked are struck in silent." (Psalm 107:42)
 *************************************************************************/
