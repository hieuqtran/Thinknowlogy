/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
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

#include "AdminItem.h"
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"
#include "SelectionItem.cpp"

class AdminSpecification
	{
	friend class AdminItem;

	// Private constructed variables

	bool isArchivedAssignment_;

	unsigned short doneSpecificationWordOrderNr_;
	unsigned short linkedGeneralizationWordTypeNr_;

	unsigned int userRelationContextNr_;

	SpecificationItem *simpleUserSpecificationItem_;
	SpecificationItem *userSpecificationItem_;

	WordItem *linkedGeneralizationWordItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;


	// Private functions

	bool isAuthorizationRequiredForChanges( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		return( generalizationWordItem != NULL &&
				specificationWordItem != NULL &&
				adminItem_->isSystemStartingUp() &&

				( generalizationWordItem->isAuthorizationRequiredForChanges() ||
				specificationWordItem->isAuthorizationRequiredForChanges() ) );
		}

	bool isValidRelationContextInContextWords( bool isPossessive, unsigned int existingRelationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *currentContextWordItem;

		if( existingRelationContextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != relationWordItem &&
				!currentContextWordItem->isUserRelationWord &&
				currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, existingRelationContextNr, specificationWordItem ) != NULL )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return true;
		}

	unsigned int collectionNrByCompoundGeneralizationWordInCollectionWords( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		WordItem *currentCollectionWordItem;

		if( collectionWordTypeNr > NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != NULL &&
		( currentCollectionWordItem = commonVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( collectionNr = currentCollectionWordItem->collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	signed char addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isCheckingExistingUserSpecification;
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		bool isStop = false;
		unsigned short relationWordTypeNr;
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *mostRecentContextWord;
		WordItem *relationWordItem;
		WordItem *previousRelationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecificationWithRelation";

		if( startRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isCheckingExistingUserSpecification = ( isPossessive &&
												nContextRelations == 0 &&
												!specificationWordItem->isNounWordSpanishAmbiguous() );

		do	{
			if( currentRelationReadItem->isPreposition() )
				prepositionParameter = currentRelationReadItem->wordParameter();
			else
				{
				if( currentRelationReadItem->isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem->readWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined read word" );

					if( specificationWordItem == relationWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "The relation word is the same as the specification word" );

					if( generalizationWordItem == relationWordItem )
						{
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						relationWordTypeNr = currentRelationReadItem->wordTypeNr();

						if( previousRelationWordItem == NULL &&
						commonVariables_->nUserRelationWords == 1 &&
						( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != NULL &&
						foundSpecificationItem->hasRelationContext() &&
						( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem->relationContextNr() ) ) != NULL &&
						mostRecentContextWord != relationWordItem )
							previousRelationWordItem = mostRecentContextWord;

						if( previousRelationWordItem != NULL &&
						collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

						if( selectionListNr == NO_LIST_NR )
							{
							existingSpecificationItem = ( isCheckingExistingUserSpecification ? generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) : NULL );

							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == NULL ? NO_COLLECTION_NR : existingSpecificationItem->specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NULL, generalizationWordItem, specificationWordItem, relationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( adminItem_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem->isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a selection part having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							isFirstComparisonPart = false;
							}

						previousRelationWordItem = relationWordItem;
						}
					}
				}
			}
		while( !isStop &&
		!commonVariables_->hasDisplayedWarning &&
		currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	signed char collectGeneralizationWords( bool isExclusiveGeneralization, unsigned short generalizationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousGeneralizationWordItem, WordItem *newGeneralizationWordItem, WordItem *previousCommonWordItem, WordItem *newCommonWordItem )
		{
		bool hasFoundCollection = false;
		unsigned int generalizationCollectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationWords";

		if( previousGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous generalization word item is undefined" );

		if( newGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given new generalization word item is undefined" );

		if( previousGeneralizationWordItem == newGeneralizationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and new generalization word items are the same word" );

		if( newCommonWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given new common word item is undefined" );

		if( ( generalizationCollectionNr = previousGeneralizationWordItem->collectionNr( generalizationWordTypeNr, newCommonWordItem ) ) == NO_COLLECTION_NR )
			generalizationCollectionNr = newGeneralizationWordItem->collectionNr( generalizationWordTypeNr, newCommonWordItem );

		if( generalizationCollectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = newGeneralizationWordItem->findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" is collected with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, newCommonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( generalizationCollectionNr == NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( newGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", newGeneralizationWordItem->anyWordTypeString(), "\" with word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( previousGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in previous generalization word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( newCommonWordItem != previousCommonWordItem )
				{
				if( previousGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, previousCommonWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );

				if( newGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, previousCommonWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", newGeneralizationWordItem->anyWordTypeString(), "\" with word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );
				}

			if( newGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in new generalization word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *commonWordItem )
		{
		bool hasFoundCollection = false;
		unsigned int collectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectRelationWords";

		if( previousRelationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		if( currentRelationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current relation word item is undefined" );

		if( previousRelationWordItem == currentRelationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and current relation words are the same word" );

		if( ( collectionNr = previousRelationWordItem->collectionNr( relationWordTypeNr, commonWordItem ) ) == NO_COLLECTION_NR )
			collectionNr = currentRelationWordItem->collectionNr( relationWordTypeNr, commonWordItem );

		if( collectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = previousRelationWordItem->findCollection( false, currentRelationWordItem, commonWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", previousRelationWordItem->anyWordTypeString(), "\" is collected with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousRelationWordItem->addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousRelationWordItem->anyWordTypeString(), "\" with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

			if( collectionNr == NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem->addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", currentRelationWordItem->anyWordTypeString(), "\" with word \"", previousRelationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		bool hasFoundCollection = false;
		unsigned short existingPairCollectionOrderNr;
		unsigned short collectionOrderNr = NO_ORDER_NR;
		unsigned int specificationCollectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( previousSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous specification word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem == currentSpecificationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and current specification words are the same word" );

		if( compoundGeneralizationWordItem == NULL )
			{
			if( ( specificationCollectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == NO_COLLECTION_NR )
				{
				if( ( specificationCollectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == NO_COLLECTION_NR )
					{
					if( ( specificationWordTypeNr != WORD_TYPE_NOUN_PLURAL ||
					generalizationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ) &&

					( specificationCollectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
						specificationCollectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr );
					}
				}
			else
				{
				// Collection already exists
				if( previousSpecificationWordItem->isOlderItem() &&
				currentSpecificationWordItem->isOlderItem() )
					{
					hasFoundCollection = true;
					specificationCollectionNr = NO_COLLECTION_NR;
					}
				}
			}
		else
			{
			if( !isExclusiveSpecification ||
			generalizationWordItem == previousSpecificationWordItem ||
			!generalizationWordItem->isNounWordSpanishAmbiguous() )
				specificationCollectionNr = collectionNrByCompoundGeneralizationWordInCollectionWords( isExclusiveSpecification, specificationWordTypeNr, compoundGeneralizationWordItem );
			}

		if( specificationCollectionNr > NO_COLLECTION_NR )
			{
			if( ( shortResult = checkCollectionInCollectionWords( specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the collection in all words" );

			if( ( existingPairCollectionOrderNr = shortResult.shortValue ) > NO_ORDER_NR &&
			( collectionOrderNr = adminItem_->highestCollectionOrderNrInCollectionWords( specificationCollectionNr ) ) > 1 &&
			// "- 1" because a collection comes in pairs
			existingPairCollectionOrderNr < collectionOrderNr - 1 )
				specificationCollectionNr = NO_COLLECTION_NR;
			}

		if( !isPossessive &&
		specificationCollectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = previousSpecificationWordItem->findCollection( ( previousSpecificationWordItem != generalizationWordItem ), currentSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", currentSpecificationWordItem->anyWordTypeString(), "\" is collected with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// A collection comes in pairs
			if( collectionOrderNr >= MAX_ORDER_NR - 1 )
				return adminItem_->startSystemError( functionNameString, moduleNameString_, "Collection order number overflow" );

			if( ( collectionResult = previousSpecificationWordItem->addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem->addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		if( specificationCollectionNr > NO_COLLECTION_NR &&
		generalizationWordItem->collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect specifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	signed char copyContext( unsigned int existingContextNr, unsigned int newContextNr )
		{
		ContextItem *currentContextItem;
		WordItem *currentContextWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyContext";

		if( ( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem->contextItem( existingContextNr ) ) != NULL &&
				currentContextWordItem->addContext( currentContextItem->isCompoundCollectionSpanishAmbiguous(), currentContextItem->contextWordTypeNr(), currentContextItem->specificationWordTypeNr(), newContextNr, currentContextItem->specificationWordItem() ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a copied context to word \"", currentContextWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processJustification( bool hasFoundRelationContext, bool hasRelationWord, bool hasRelationWordConfirmedSpecification, bool isUserGeneralizationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool isAttachingJustification = true;
		bool isDeletingCreatedJustification = false;
		bool isRemovingPreviousJustifications = false;
		bool isSpecificationWordSpanishAmbiguous;
		JustificationResultType justificationResult;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem *existingJustificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *confirmedAllRelationWordsOfSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processJustification";

		if( createdOrFoundJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem->isSpecificationWordSpanishAmbiguous();

		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				if( relationWordItem != NULL )
					{
					if( secondarySpecificationItem->isSelfGeneratedAssumption() )
						{
						// Skip if justification with self-generated assumption is created,
						// while justification with self-generated conclusion already exists
						if( relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), false, secondarySpecificationItem->specificationWordItem(), generalizationWordItem ) != NULL )
							isDeletingCreatedJustification = true;
						}
					else
						{
						if( isUserGeneralizationWord )
							{
							if( isSpecificationWordSpanishAmbiguous )
								{
								if( !foundSpecificationItem->isOlderItem() &&
								foundSpecificationItem->isHiddenSpanishSpecification() &&
								createdOrFoundJustificationItem->hasFeminineOrMasculineProperNameEnding() )
									isAttachingJustification = false;
								}
							else
								{
								if( secondarySpecificationItem->isSelfGeneratedConclusion() &&
								( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL &&
								( existingJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != NULL )
									{
									if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									isAttachingJustification = false;
									}
								}
							}
						}
					}

				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				hasRelationWord &&
				isUserGeneralizationWord &&
				primarySpecificationItem != NULL &&
				// Confirmation
				secondarySpecificationItem->isUserSpecification() )
					{
					if( ( existingJustificationItem = generalizationWordItem->olderComplexJustificationItem( true, justificationTypeNr, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) == NULL )
						{
						if( ( obsoleteJustificationItem = generalizationWordItem->possessiveReversibleAssumptionJustificationItem( relationWordItem, secondarySpecificationWordItem ) ) == NULL )
							{
							// Skip if justification already exists as an opposite possessive conditional specification assumption
							if( secondarySpecificationItem->isGeneralizationProperName() &&
							generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) != NULL )
								isDeletingCreatedJustification = true;
							}
						else
							{
							if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, justificationTypeNr, obsoleteJustificationItem->orderNr, obsoleteJustificationItem->originalSentenceNr(), NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification without primary specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find or create a justification without primary specification" );

							if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						// Replace existing justification by the created justification
						if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						isAttachingJustification = false;
						}
					}

				break;

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				if( !hasRelationWordConfirmedSpecification &&
				!isUserGeneralizationWord &&
				foundSpecificationItem->isOlderItem() &&
				foundSpecificationItem->isSelfGeneratedConclusion() )
					isDeletingCreatedJustification = true;

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != NULL &&
				( existingJustificationItem = foundSpecificationItem->differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) != NULL )
					{
					isAttachingJustification = false;

					if( primarySpecificationItem->assumptionLevel() < existingJustificationItem->primarySpecificationAssumptionLevel() )
						{
						// Replace less certain existing justification by the created justification
						if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						isDeletingCreatedJustification = true;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != NULL &&
				( specificationSubstitutionPartOfAssumptionJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != NULL )
					{
					if( specificationSubstitutionPartOfAssumptionJustificationItem->hasHiddenPrimarySpecification() )
						{
						if( specificationSubstitutionPartOfAssumptionJustificationItem->changePrimarySpecification( primarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a hidden primary specification of the attached justification item of the found specification substitution 'part of' assumption justification item" );

						isDeletingCreatedJustification = true;
						}
					}

				break;

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				if( ( !isSpecificationWordSpanishAmbiguous ||
				foundSpecificationItem->isConcludedAssumption() ) &&

				foundSpecificationItem->hasOnlyOneRelationWord() )
					isRemovingPreviousJustifications = true;
				else
					{
					if( hasRelationWordConfirmedSpecification &&
					foundSpecificationItem->isSelfGeneratedAssumption() &&
					( confirmedAllRelationWordsOfSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, foundSpecificationItem->specificationWordItem(), NULL ) ) != NULL )
						{
						if( confirmedAllRelationWordsOfSpecificationItem->attachJustificationToSpecification( createdOrFoundJustificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the conclusion specification" );

						isAttachingJustification = false;
						}
					}

				break;

			case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				if( anotherSecondarySpecificationItem != NULL &&
				foundSpecificationItem->isConcludedAssumption() &&
				( obsoleteJustificationItem = generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, secondarySpecificationItem ) ) != NULL )
					{
					if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace negative assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					isAttachingJustification = false;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Generalization word has adjusted question
				if( foundSpecificationItem->isOlderItem() &&
				adminItem_->adjustedQuestionWordItem() == generalizationWordItem )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
					// Adjusted question. Needs to be rewritten
					strcpy( foundSpecificationItem->lastWrittenSentenceString, EMPTY_STRING );
					}
			}

		if( isDeletingCreatedJustification )
			{
			if( generalizationWordItem->replaceOrDeleteJustification( createdOrFoundJustificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			if( isRemovingPreviousJustifications )
				{
				if( generalizationWordItem->copyAndReplaceSpecificationItem( foundSpecificationItem->isAnsweredQuestion(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveSpecification(), foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy and replace a specification with a different first justification item" );
				}
			else
				{
				if( isAttachingJustification )
					{
					if( primarySpecificationItem == NULL &&
					foundSpecificationItem->isSelfGeneratedAssumption() &&
					!foundSpecificationItem->hasAssumptionLevel() )
						foundSpecificationItem->incrementAssumptionLevel();

					// Attach created justification to the found specification
					if( generalizationWordItem->attachJustificationInWord( createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char recalculateAssumptionsInTouchedWords()
		{
		WordItem *currentTouchedProperNameWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumptionsInTouchedWords";

		if( ( currentTouchedProperNameWordItem = commonVariables_->firstTouchedProperNameWordItem ) != NULL )
			{
			// Do for all proper name words touched during current sentence
			do	{
				if( currentTouchedProperNameWordItem->recalculateAssumptionsInWord() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumptions of word \"", currentTouchedProperNameWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem->nextTouchedProperNameWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	WordItem *mostRecentContextWordInContextWords( unsigned int contextNr )
		{
		ContextItem *currentContextItem;
		ContextItem *mostRecentContextItem = NULL;
		WordItem *currentContextWordItem;
		WordItem *mostRecentWordItem = NULL;

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				currentContextItem = currentContextWordItem->contextItem( contextNr );

				if( currentContextItem != NULL &&

				( mostRecentContextItem == NULL ||
				currentContextItem->isMoreRecent( mostRecentContextItem ) ) )
					{
					mostRecentWordItem = currentContextWordItem;
					mostRecentContextItem = currentContextItem;
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return mostRecentWordItem;
		}

	ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( bool isPossessive, unsigned int nContextRelations, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		ContextResultType contextResult;
		unsigned int currentRelationContextNr;
		unsigned int nContextWords;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveReversibleConclusionRelationContextOfInvolvedWords";

		if( relationContextNr <= NO_CONTEXT_NR )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = generalizationWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( currentSpecificationItem = currentGeneralizationWordItem->firstRelationSpecificationItem() ) != NULL )
					{
					do	{
						if( currentSpecificationItem->isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
							{
							currentRelationContextNr = currentSpecificationItem->relationContextNr();

							if( !isPossessive ||
							currentRelationContextNr != relationContextNr )
								{
								nContextWords = adminItem_->nContextWordsInContextWords( currentRelationContextNr, specificationWordItem );

								if( currentSpecificationItem->isPossessiveReversibleConclusion() )
									{
									// Has found all relation words.
									// In other words: This relation word will be the last one
									if( nContextWords + 1 == nContextRelations )
										{
										contextResult.contextNr = relationContextNr;
										contextResult.confirmedAllRelationWordsOfSpecificationItem = foundSpecificationItem;
										}
									else
										contextResult.contextNr = currentRelationContextNr;
									}
								else
									{
									// Found equal number of relation words
									if( nContextWords == nContextRelations )
										{
										contextResult.contextNr = currentRelationContextNr;
										contextResult.confirmedAllRelationWordsOfSpecificationItem = foundSpecificationItem;
										}
									}
								}
							}
						}
					while( contextResult.contextNr == NO_CONTEXT_NR &&
					( currentSpecificationItem = currentSpecificationItem->nextRelationSpecificationItem() ) != NULL );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
			}

		return contextResult;
		}

	ContextResultType getRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCompoundCollectionSpanishAmbiguous, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int specificationCollectionNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		bool hasGeneralizationWordAnsweredSelfGeneratedQuestion;
		bool hasRelationWordConfirmedSpecification;
		bool hasRelationWordExistingRelationContext;
		bool isExistingSpecificationConcludedAssumption;
		bool isExistingSpecificationSelfGeneratedConclusion;
		bool isGeneralizationWordUserRelationWord;
		bool isPossessiveUserSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int foundContextNr;
		unsigned int existingCopyContextNr = NO_CONTEXT_NR;
		unsigned int existingRelationContextNr = NO_CONTEXT_NR;
		unsigned int foundRelationContextNr = NO_CONTEXT_NR;
		unsigned int nUserRelationWords = commonVariables_->nUserRelationWords;
		ContextItem *foundContextItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( isCompoundCollectionSpanishAmbiguous )
			existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, specificationWordItem );
		else
			existingSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

		hasRelationWordConfirmedSpecification = relationWordItem->hasConfirmedSpecification();

		if( existingSpecificationItem != NULL )
			existingRelationContextNr = existingSpecificationItem->relationContextNr();

		// Try to find relation context with same number of relation words as in the user sentence
		if( ( foundContextItem = relationWordItem->contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, specificationWordItem ) ) == NULL )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( existingSpecificationItem == NULL ||

			( !hasRelationWordConfirmedSpecification &&
			isGeneralizationWordUserRelationWord &&
			!isSpecificationWordSpanishAmbiguous &&
			!existingSpecificationItem->hasRelationContext() ) )
				{
				if( nUserRelationWords > 1 &&

				( commonVariables_->nUserGeneralizationWords > 1 ||
				!relationWordItem->isOlderItem() ||
				// Typically for the Spanish language
				generalizationWordItem->isFemale() ) &&

				( foundContextNr = relationWordItem->contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem ) ) > NO_CONTEXT_NR )
					contextResult.contextNr = foundContextNr;
				}
			else
				{
				isExistingSpecificationConcludedAssumption = existingSpecificationItem->isConcludedAssumption();

				// Not found yet
				if( contextResult.contextNr == NO_CONTEXT_NR )
					{
					// Find specification with found context word as relation word
					if( isSpecificationWordSpanishAmbiguous )
						{
						if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
							{
							if( existingRelationContextNr > NO_CONTEXT_NR &&

							( !foundSpecificationItem->hasRelationContext() ||
							existingSpecificationItem->isHiddenSpanishSpecification() ) )
								foundRelationContextNr = existingRelationContextNr;
							else
								{
								if( specificationCollectionNr == NO_COLLECTION_NR ||
								foundSpecificationItem->isUserSpecification() ||
								foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
									foundRelationContextNr = foundSpecificationItem->relationContextNr();
								}
							}
						}
					else
						{
						if( existingRelationContextNr > NO_CONTEXT_NR &&

						( !isExistingSpecificationConcludedAssumption ||
						isGeneralizationWordUserRelationWord ) &&

						existingSpecificationItem->isArchivedAssignment() == isArchivedAssignment )
							foundRelationContextNr = existingRelationContextNr;
						}

					// Cross-collected afterwards
					if( foundRelationContextNr > NO_CONTEXT_NR &&
					contextResult.contextNr == NO_CONTEXT_NR )
						{
						isExistingSpecificationSelfGeneratedConclusion = existingSpecificationItem->isSelfGeneratedConclusion();

						if( generalizationWordItem->hasConfirmedSpecification() )
							{
							if( !isPossessive ||
							specificationCollectionNr == NO_COLLECTION_NR ||
							!existingSpecificationItem->isOlderItem() ||

							( userSpecificationItem != NULL &&
							userSpecificationItem->hasSpecificationCompoundCollection() &&

							( !isSpecificationWordSpanishAmbiguous ||

							// Typically for Spanish
							( !isExistingSpecificationSelfGeneratedConclusion &&
							userSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) )
								contextResult.contextNr = existingRelationContextNr;
							}
						else
							{
							hasRelationWordExistingRelationContext = relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem );
							isPossessiveUserSpecification = ( userSpecificationItem != NULL &&
															userSpecificationItem->isPossessive() );

							// Check for copy context
							if( !hasRelationWordExistingRelationContext &&
							isGeneralizationWordUserRelationWord &&
							existingSpecificationItem->isOlderItem() &&

							( ( isExistingSpecificationConcludedAssumption &&
							isPossessiveUserSpecification ) ||

							( !isPossessive &&
							isExistingSpecificationSelfGeneratedConclusion &&
							isSelfGeneratedAssumption &&
							!isSpecificationWordSpanishAmbiguous &&

							( existingSpecificationItem->hasSpecificationCompoundCollection() ||

							( userSpecificationItem != NULL &&

							( userSpecificationItem->hasSpecificationCompoundCollection() ||
							!userSpecificationItem->hasSpecificationCollection() ) ) ) ) ||
							
							( !isExistingSpecificationConcludedAssumption &&
							!isValidRelationContextInContextWords( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) &&
							adminItem_->nContextWordsInContextWords( existingRelationContextNr, specificationWordItem ) > commonVariables_->nUserGeneralizationWords ) ) )
								// Need to copy context (1)
								existingCopyContextNr = existingRelationContextNr;
							else
								{
								if( isExistingSpecificationConcludedAssumption ||
								isSpecificationWordSpanishAmbiguous ||

								( isExistingSpecificationSelfGeneratedConclusion &&
								isPossessive &&
								isPossessiveUserSpecification ) )
									contextResult.contextNr = foundRelationContextNr;
								else
									{
									if( existingSpecificationItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
										{
										if( isPossessive ||
										isArchivedAssignment ||
										isSelfGeneratedAssumption )
											contextResult.contextNr = existingRelationContextNr;
										else
											{
											if( isExistingSpecificationSelfGeneratedConclusion )
												{
												if( isValidRelationContextInContextWords( false, existingRelationContextNr, specificationWordItem, relationWordItem ) )
													contextResult.contextNr = existingRelationContextNr;
												}
											else
												// Need to copy context (2)
												existingCopyContextNr = existingRelationContextNr;
											}
										}
									else
										{
										hasGeneralizationWordAnsweredSelfGeneratedQuestion = generalizationWordItem->hasAnsweredSelfGeneratedQuestion();

										// Possessive
										if( ( isPossessive &&

										( specificationCollectionNr == NO_COLLECTION_NR ||

										( hasGeneralizationWordAnsweredSelfGeneratedQuestion &&
										!isGeneralizationWordUserRelationWord ) ) ) ||

										// Not possessive
										( !isPossessive &&
										!hasRelationWordExistingRelationContext &&
										!isSelfGeneratedAssumption &&

										( isExistingSpecificationSelfGeneratedConclusion ||
										isValidRelationContextInContextWords( false, existingRelationContextNr, specificationWordItem, relationWordItem ) ) ) ||

										// Self-generated assumption
										( isGeneralizationWordUserRelationWord &&

										( isPossessive ||
										isSelfGeneratedAssumption ) &&

										( !hasGeneralizationWordAnsweredSelfGeneratedQuestion ||
										hasRelationWordExistingRelationContext ||

										( isPossessive &&
										!isSpecificationWordSpanishAmbiguous &&
										isValidRelationContextInContextWords( true, existingRelationContextNr, specificationWordItem, relationWordItem ) ) ) ) )
											contextResult.contextNr = existingRelationContextNr;
										}
									}
								}
							}
						}
					}
				}
			}
		else
			{
			if( hasRelationWordConfirmedSpecification &&
			isSelfGeneratedAssumption &&
			existingSpecificationItem != NULL &&
			existingSpecificationItem->hasRelationContext() &&
			existingRelationContextNr != contextResult.contextNr )
				{
				// Both existing specification and wanted specification are assumptions
				if( existingSpecificationItem->isSelfGeneratedAssumption() )
					contextResult.contextNr = existingRelationContextNr;
				else
					// Need to copy context (3)
					existingCopyContextNr = existingRelationContextNr;
				}
			else
				{
				foundContextNr = foundContextItem->contextNr();

				if( isAssignment &&
				!isInactiveAssignment &&
				!isArchivedAssignment &&
				isPossessive &&
				// Active assignment doesn't exist
				generalizationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, true, foundContextNr, specificationWordItem ) == NULL )
					{
					// Dynamic semantic ambiguity
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
						return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					{
					if( isSelfGeneratedAssumption &&
					existingSpecificationItem != NULL &&

					( !isPossessive ||
					specificationCollectionNr == NO_COLLECTION_NR ) &&

					userSpecificationItem != NULL &&
					userSpecificationItem->isAssignment() &&
					existingSpecificationItem->hasRelationContext() &&
					existingSpecificationItem->isSelfGeneratedAssumption() )
						contextResult.contextNr = existingSpecificationItem->relationContextNr();
					else
						contextResult.contextNr = foundContextNr;
					}
				}
			}

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;

			if( existingCopyContextNr > NO_CONTEXT_NR )
				{
				if( copyContext( existingCopyContextNr, contextResult.contextNr ) != RESULT_OK )
					return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to copied a context" );

				contextResult.copiedRelationContextNr = existingCopyContextNr;
				}
			}

		return contextResult;
		}

	CreateAndAssignResultType addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasDisplayedMoreSpecificNonExclusiveSpecification;
		bool isDefinitionSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isCompoundCollectionSpanishAmbiguous = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationContextAlreadyDefined = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdSpecificationItem = NULL;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignCheckResult;
		CreateAndAssignResultType createAndAssignReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word is the administrator" );

		existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

		// Get relation context
		// Exceptions: Conditions and imperative verbs have no relation context
		if( !isConditional &&
		!isSelection &&
		generalizationWordTypeNr != WORD_TYPE_VERB_SINGULAR &&
		specificationWordItem != NULL &&
		relationWordItem != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( isSpecificationWordSpanishAmbiguous &&
			isPossessive &&
			specificationCollectionNr > NO_COLLECTION_NR &&
			specificationWordItem->isCompoundCollection( specificationCollectionNr ) )
				isCompoundCollectionSpanishAmbiguous = true;

			if( relationContextNr == NO_CONTEXT_NR ||

			( isPossessive &&
			!isSpecificationWordSpanishAmbiguous &&
			specificationCollectionNr == NO_COLLECTION_NR &&

			( nContextRelations == 0 ||
			!generalizationWordItem->isOlderItem() ) &&

			// Possessive user specification
			( existingSpecificationItem == NULL ||

			( isAssignment &&
			!isArchivedAssignment ) ||

			// Ignore context in other words
			( existingSpecificationItem->hasRelationContext() &&
			existingSpecificationItem->relationContextNr() != relationContextNr &&

			( !relationWordItem->hasConfirmedSpecification() ||
			!existingSpecificationItem->hasSpecificationCollection() ||
			existingSpecificationItem->isSelfGeneratedAssumption() ) ) ) ) ||

			// User assignment, and existing self-generated specification
			( isAssignment &&
			!isPossessive &&
			userSpecificationItem_ != NULL &&
			!userSpecificationItem_->isOlderItem() &&

			( existingSpecificationItem == NULL ||
			existingSpecificationItem->isOlderItem() ) ) )
				{
				if( ( contextResult = getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isPossessive, ( firstJustificationItem != NULL && firstJustificationItem->isAssumptionJustification() ), specificationCollectionNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to get the relation context" );

				relationContextNr = contextResult.contextNr;
				}
			else
				{
				isRelationContextAlreadyDefined = true;

				// User assignment, and existing user assignment
				if( isAssignment &&
				!isPossessive &&
				userSpecificationItem_ != NULL &&
				existingSpecificationItem != NULL &&
				existingSpecificationItem->isUserAssignment() &&
				existingSpecificationItem->relationContextNr() != relationContextNr )
					relationContextNr = existingSpecificationItem->relationContextNr();
				}

			if( relationWordItem->addContext( isCompoundCollectionSpanishAmbiguous, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
			}

		// Skip adding specification if already exists
		if( isAssignment ||
		!isRelationContextAlreadyDefined ||
		existingSpecificationItem == NULL ||
		!existingSpecificationItem->isActiveAssignment() )
			{
			// Check for an assignment or a specification with opposite negative indicator
			if( !isAssignment &&
			isArchivedAssignment_ &&
			generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, !isNegative, isPossessive, questionParameter, specificationWordItem ) != NULL )
				isAssignment = true;

			if( adminItem_->isSystemStartingUp() &&
			isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
				{
				if( ( createAndAssignReturnResult = adminItem_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with authorization" );
				}
			else
				{
				if( ( createAndAssignReturnResult = generalizationWordItem->addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add an assignment specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				replacedAssignmentItem = createAndAssignReturnResult.replacedAssignmentItem;

				isDefinitionSpecification = ( !hasRelationWord &&
											!isQuestion &&
											adminItem_->isNounWordType( generalizationWordTypeNr ) &&
											adminItem_->isNounWordType( specificationWordTypeNr ) );

				if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == NULL )
					{
					if( hasRelationWord &&
					!isSelfGenerated &&
					( foundSpecificationItem = createAndAssignReturnResult.foundSpecificationItem ) != NULL )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem->hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, specificationWordItem );
						}
					}
				else
					{
					if( !isSelfGenerated )
						{
						userSpecificationItem_ = createdSpecificationItem;

						if( !isQuestion )
							simpleUserSpecificationItem_ = createdSpecificationItem;
						}
					}

				if( !isExclusiveSpecification &&
				!isQuestion &&
				!isSelfGenerated &&
				specificationWordItem != NULL &&
				!generalizationWordItem->hasConfirmedSpecification() )
					{
					if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
						{
						if( !isSpecificationGeneralization &&
						createdSpecificationItem != NULL )
							{
							if( isNegative )
								{
								if( createdSpecificationItem->hasSpecificationCollection() &&
								adminItem_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an only-option-left assumption" );
								}
							else
								{
								if( !isPossessive )
									{
									if( isAssignment &&
									hasRelationWord &&
									// Collect generalization words of a proper name specification
									adminItem_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

									if( adminItem_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									hasDisplayedMoreSpecificNonExclusiveSpecification = generalizationWordItem->hasDisplayedMoreSpecificNonExclusiveSpecification();

									if( generalizationWordItem->hasFoundAnswerToQuestion() &&

									( hasDisplayedMoreSpecificNonExclusiveSpecification ||
									!specificationWordItem->isCompoundCollection( createdSpecificationItem->specificationCollectionNr() ) ) &&

									adminItem_->drawNegativeConclusionsFromAnsweredQuestions( hasDisplayedMoreSpecificNonExclusiveSpecification, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw negative conclusions from answered questions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					else
						{
						if( isDefinitionSpecification )
							{
							if( isSpecificationGeneralization )
								{
								if( isPartOf &&
								!isPossessive &&

								( !isAssignment ||
								isArchivedAssignment ) &&

								// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
								// Not yet explained in the design
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, false, true, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								if( createdSpecificationItem == NULL )
									{
									if( !isConditional &&
									!isNegative )
										{
										// Check if primary specification already has an assignment
										foundAssignmentItem = generalizationWordItem->firstAssignmentItem( true, true, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );

										if( isAssignment ||
										foundAssignmentItem != NULL )
											{
											if( generalizationWordItem->writeSelectedSpecification( false, true, ( foundAssignmentItem == NULL ? createAndAssignReturnResult.foundSpecificationItem : foundAssignmentItem ) ) != RESULT_OK )
												return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a non-proper-name specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( strlen( commonVariables_->writtenSentenceString ) == 0 )
												return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

											if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( foundAssignmentItem == NULL ? INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != RESULT_OK )
												return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a header" );

											if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
												return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a sentence" );
											}
										}
									}
								else
									{
									if( isPossessive &&

									( !isAssignment ||
									isArchivedAssignment ) &&

									// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
									// Not yet explained in the design
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, true, false, false, false, true, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					}

				if( !commonVariables_->hasDisplayedWarning &&

				( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != NULL ) &&

				// Skip for example imperative verbs
				adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
					{
					if( ( createAndAssignCheckResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != NULL && specificationWordItem->isAdjectiveAssignedOrEmpty() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != NULL )
						{
						if( !isArchivedAssignment_ ||
						createAndAssignReturnResult.createdSpecificationItem != NULL )
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != NULL &&
						generalizationWordItem->replaceOrDeleteSpecification( false, replacedAssignmentItem, createdAssignmentItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( isNegative &&
						!isQuestion &&
						createdAssignmentItem->hasSpecificationCollection() &&
						generalizationWordItem->isUserGeneralizationWord &&
						adminItem_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
						}

					if( !isSelfGenerated &&
					( tempSpecificationItem = ( createdAssignmentItem == NULL ? createAndAssignCheckResult.foundSpecificationItem : createdAssignmentItem ) ) != NULL )
						{
						userSpecificationItem_ = tempSpecificationItem;

						if( isArchivedAssignment_ &&
						!isQuestion )
							simpleUserSpecificationItem_ = tempSpecificationItem;
						}
					}

				if( !commonVariables_->hasDisplayedWarning &&
				!isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				replacedAssignmentItem == NULL &&
				specificationWordItem != NULL &&
				userSpecificationItem_ != NULL &&

				( !isSelfGenerated ||

				( isDefinitionSpecification &&
				isEveryGeneralization &&
				!isPossessive &&
				!userSpecificationItem_->hasSpecificationCollection() ) ) )
					{
					if( isQuestion )
						{
						if( hasRelationWord &&

						( createdAssignmentItem != NULL ||

						( createdSpecificationItem != NULL &&
						!generalizationWordItem->hasDisplayedMoreSpecificQuestion() ) ) )
							{
							if( adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == NULL ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						if( isDefinitionSpecification &&
						adminItem_->drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion" );
						}
					}
				}
			}

		return createAndAssignReturnResult;
		}

	ShortResultType checkCollectionInCollectionWords( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		unsigned short existingPairCollectionOrderNr = NO_ORDER_NR;
		unsigned short foundCollectionOrderNr = NO_ORDER_NR;
		unsigned int nWords = 0;
		WordItem *currentCollectionWordItem;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCollectionInCollectionWords";

		if( collectionNr <= NO_COLLECTION_NR )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given collection number is undefined" );

		if( collectionWordItem == NULL )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentCollectionWordItem = commonVariables_->firstCollectionWordItem ) == NULL )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given first collection word item is undefined" );

		// Do for all collection words
		do	{
			if( ( foundCollectionOrderNr = currentCollectionWordItem->collectionOrderNr( collectionNr, collectionWordItem, commonWordItem ) ) > NO_ORDER_NR )
				{
				// Found existing collection pair
				if( ++nWords == 2 )
					existingPairCollectionOrderNr = foundCollectionOrderNr;
				}
			}
		while( existingPairCollectionOrderNr == NO_ORDER_NR &&
		( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );

		shortResult.shortValue = existingPairCollectionOrderNr;
		return shortResult;
		}


	protected:
	// Constructor

	AdminSpecification( AdminItem *adminItem, CommonVariables *commonVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		userRelationContextNr_ = NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		linkedGeneralizationWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminSpecification" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected assignment functions

	signed char assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( assignmentSelectionItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem->isAssignedOrEmpty(), assignmentSelectionItem->isInactiveAssignment(), assignmentSelectionItem->isArchivedAssignment(), assignmentSelectionItem->isNegative(), false, assignmentSelectionItem->isPossessive(), assignmentSelectionItem->isSpecificationGeneralization(), assignmentSelectionItem->isUniqueUserRelation(), assignmentSelectionItem->assumptionLevel(), assignmentSelectionItem->prepositionParameter(), NO_QUESTION_PARAMETER, assignmentSelectionItem->relationWordTypeNr(), assignmentSelectionItem->generalizationContextNr(), assignmentSelectionItem->specificationContextNr(), assignmentSelectionItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, assignmentSelectionItem->nContextRelations(), NULL, assignmentSelectionItem->generalizationWordItem(), assignmentSelectionItem->specificationWordItem(), assignmentSelectionItem->specificationString() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a specification" );

		return RESULT_OK;
		}

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_->isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

		return generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL );
		}


	// Protected specification functions

	void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		linkedGeneralizationWordItem_ = NULL;
		}

	void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		userRelationContextNr_ = NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordItem_ = NULL;
		}

	ContextResultType getGeneralizationContext( bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		ContextResultType contextResult;
		bool hasFoundRelationContext;
		bool hasFoundRelationWordInThisList;
		bool hasSameNumberOrMoreRelationWords;
		bool isSkippingThisContext = false;
		unsigned int currentRelationContextNr;
		unsigned int foundRelationContextNr;
		ContextItem *currentRelationContextItem;
		ReadItem *relationWordReadItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *currentRelationWordItem = relationWordItem;
		WordItem *firstContextWordItem = commonVariables_->firstContextWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getGeneralizationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != NULL )
			{
			if( ( relationWordReadItem = startRelationReadItem->firstRelationWordReadItem() ) == NULL )
				return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationWordReadItem->readWordItem();
			}

		if( currentRelationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem->firstActiveContextItem() ) != NULL )
			{
			// Do for all relation context items in the first relation context word
			do	{
				currentRelationContextNr = currentRelationContextItem->contextNr();

				if( currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					hasFoundRelationWordInThisList = false;
					isSkippingThisContext = false;

					if( firstContextWordItem != NULL )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words, either in the current relation list or outside this list
						do	{
							foundSpecificationItem = ( isUserSentence ? NULL : generalizationWordItem->firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) );

							if( foundSpecificationItem == NULL ||
							!foundSpecificationItem->isSelfGeneratedConclusion() )
								{
								if( relationWordReadItem != NULL )
									hasFoundRelationWordInThisList = relationWordReadItem->hasFoundRelationWordInThisList( currentContextWordItem );

								hasFoundRelationContext = currentContextWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem );

								// Word is one of the relation words in this list, but doesn't have current context
								if( ( !hasFoundRelationContext &&
								hasFoundRelationWordInThisList ) ||

								// Word is in not current list of relation words, but has current context
								( hasFoundRelationContext &&
								!hasFoundRelationWordInThisList ) )
									isSkippingThisContext = true;
								}
							}
						while( !isSkippingThisContext &&
						( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
						}

					// The relation words in the list contain this context exclusively. (So, no other words)
					if( !isSkippingThisContext )
						contextResult.contextNr = currentRelationContextNr;
					}
				else
					{
					if( isUserSentence &&
					!currentRelationContextItem->isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem->nextContextItem() ) != NULL );
			}

		if( !isQuestion &&
		contextResult.contextNr == NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, NULL ) ) != NULL )
			{
			if( ( foundRelationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
				{
				hasSameNumberOrMoreRelationWords = ( adminItem_->nContextWordsInContextWords( foundRelationContextNr, specificationWordItem ) >= commonVariables_->nUserRelationWords );

				if( hasSameNumberOrMoreRelationWords ||
				generalizationWordItem->isUserRelationWord )
					{
					if( foundSpecificationItem->isExclusiveSpecification() )
						{
						// Already existing static (exclusive) semantic ambiguity
						contextResult.isAmbiguousRelationContext = true;
						contextResult.contextNr = foundRelationContextNr;
						}
					else
						{
						if( isUserSentence )
							{
							if( !isSkippingThisContext )
								{
								contextResult.contextNr = foundRelationContextNr;

								if( !foundSpecificationItem->isActiveAssignment() )
									{
									// Static (exclusive) semantic ambiguity
									if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
										return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

									contextResult.isAmbiguousRelationContext = true;
									}
								}
							}
						else
							{
							contextResult.contextNr = foundRelationContextNr;

							if( hasSameNumberOrMoreRelationWords &&
							// Try to find the relation context of a possessive reversible conclusion
							( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
								return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
							}
						}
					}
				}
			}

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		return contextResult;
		}

	signed char addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		bool isGeneralizationProperName;
		bool isGeneralizationReasoningWordType;
		bool isRelationWord;
		bool isSpecificationWord;
		bool hasAddedSpecification = false;
		bool hasRelationWord = ( startRelationReadItem != NULL );
		bool hasLinkedPossessiveDeterminer = false;
		bool isAmbiguousRelationContext = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSelection = ( selectionListNr != NO_LIST_NR );
		bool isSkippingThisGeneralizationPart = false;
		bool isSpecificationWordAlreadyAssignedByComparison = false;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short firstSpecificationWordOrderNr;
		unsigned short generalizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short valueGeneralizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short linkedSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short currentSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short previousSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned int nContextRelations = 0;
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		WordItem *generalizationWordItem;
		WordItem *readWordItem;
		WordItem *relationWordItem;
		WordItem *tempSpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *currentSpecificationWordItem = NULL;
		WordItem *linkedSpecificationWordItem = NULL;
		WordItem *previousSpecificationWordItem = NULL;
		WordItem *valueGeneralizationWordItem = NULL;
		char *specificationString = NULL;
		CompoundResultType compoundResult;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecifications";

		if( generalizationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item is undefined" );

		if( ( generalizationWordItem = generalizationReadItem->readWordItem() ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item has no read word" );

		if( ( currentReadItem = startSpecificationReadItem ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given start specification read item is undefined" );

		if( endSpecificationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given end specification read item is undefined" );

		isGeneralizationProperName = generalizationReadItem->isProperName();
		generalizationWordTypeNr = generalizationReadItem->wordTypeNr();
		isGeneralizationReasoningWordType = adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr );

		lastGeneralizationReadItem = generalizationReadItem->nextReadItem();

		while( lastGeneralizationReadItem != NULL &&
		!lastGeneralizationReadItem->isGeneralizationWord() )
			{
			if( lastGeneralizationReadItem->isPossessiveDeterminer() )
				hasLinkedPossessiveDeterminer = true;

			lastGeneralizationReadItem = lastGeneralizationReadItem->nextReadItem();
			}

		firstSpecificationWordOrderNr = ( lastGeneralizationReadItem == NULL ? endSpecificationReadItem->wordOrderNr() : (unsigned short)( lastGeneralizationReadItem->wordOrderNr() + 1 ) );

		do	{
			isRelationWord = currentReadItem->isRelationWord();
			isSpecificationWord = currentReadItem->isSpecificationWord();

			if( isSkippingThisGeneralizationPart )
				{
				if( isWaitingForRelation )
					{
					if( isRelationWord ||
					currentReadItem->isText() )
						{
						isSkippingThisGeneralizationPart = false;
						isWaitingForRelation = false;
						isWaitingForText = false;
						}
					}
				else
					{
					if( hasRelationWord &&
					generalizationReadItem->wordOrderNr() < currentReadItem->wordOrderNr() )
						isSkippingThisGeneralizationPart = false;
					}
				}
			else
				{
				if( hasRelationWord &&
				doneSpecificationWordOrderNr_ > currentReadItem->wordOrderNr() )
					isSkippingThisGeneralizationPart = true;
				else
					{
					if( currentReadItem->isGeneralizationWord() )
						{
						if( hasRelationWord ||
						isSelection )
							{
							isSkippingThisGeneralizationPart = true;
							isWaitingForRelation = true;
							}
						}
					else
						{
						if( isSpecificationWord &&
						generalizationWordItem->isAdjectiveComparison() &&
						( tempSpecificationWordItem = currentReadItem->readWordItem() ) != NULL )
							{
							// Skip head and tail in the comparison
							if( !tempSpecificationWordItem->isNounHead() &&
							!tempSpecificationWordItem->isNounTail() )
								{
								isWaitingForText = true;
								isSpecificationWordAlreadyAssignedByComparison = true;
								currentSpecificationWordTypeNr = currentReadItem->wordTypeNr();
								currentSpecificationWordItem = tempSpecificationWordItem;
								}
							}
						}
					}
				}

			if( !isWaitingForText &&
			!isSkippingThisGeneralizationPart &&
			!currentReadItem->isNegative() &&
			// Skip numeral 'both'. Typically for English: in both ... and ...
			!currentReadItem->isNumeralBoth() )
				{
				readWordItem = currentReadItem->readWordItem();
				specificationString = NULL;

				if( currentReadItem->isText() )
					specificationString = currentReadItem->readString;

				if( isSpecificationWordAlreadyAssignedByComparison )
					isSpecificationWordAlreadyAssignedByComparison = false;
				else
					{
					currentSpecificationWordTypeNr = ( isQuestion && currentReadItem->isNoun() ? WORD_TYPE_NOUN_SINGULAR : currentReadItem->wordTypeNr() );
					currentSpecificationWordItem = readWordItem;
					}

				if( isPossessive &&
				currentReadItem->isNumeral() )
					sscanf( currentReadItem->readWordTypeString(), "%u", &nContextRelations );
				else
					{
					if( !isSelection &&
					isSpecificationWord &&
					startRelationReadItem != NULL &&
					!generalizationWordItem->isVerbImperativeDisplayOrLoginOrRead() )
						{
						if( ( contextResult = getGeneralizationContext( isArchivedAssignment, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, startRelationReadItem ) ).result != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to get the multiple entry relation context" );

						isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
						userRelationContextNr_ = contextResult.contextNr;
						}

					if( currentReadItem->isNumeral() ||
					currentReadItem->isText() ||

					( isSpecificationWord &&
					!currentReadItem->isPreposition() ) )
						{
						if( isValueSpecificationWord )
							{
							hasAddedSpecification = true;

							if( hasRelationWord )
								return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Adding a value specification with relation isn't implemented" );

							if( isSelection &&
							adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a value selection item" );

							// Value, but no relation
							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a value specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( currentReadItem->isNounValue() )
								{
								isValueSpecificationWord = true;
								valueGeneralizationWordTypeNr = generalizationWordTypeNr;
								valueGeneralizationWordItem = generalizationWordItem;
								}
							else
								{
								if( isSelection )
									{
									hasAddedSpecification = true;

									if( hasRelationWord )
										{
										// Selection, no value, with relation
										if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, selectionLevel, selectionListNr, imperativeVerbParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else
										{
										// Selection, no value, no relation
										if( adminItem_->createSelectionPart( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationWordItem != NULL &&
									currentSpecificationWordItem != NULL &&
									currentReadItem->isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to collect specification words" );

									if( currentSpecificationWordTypeNr != WORD_TYPE_PREPOSITION )
										{
										previousSpecificationWordItem = currentSpecificationWordItem;
										previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
										}
									}
								}

							if( !isSelection ||
							!currentReadItem->isAdjectiveAssignedOrEmpty() )
								{
								doneSpecificationWordOrderNr_ = currentReadItem->wordOrderNr();
								linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
								linkedSpecificationWordItem = currentSpecificationWordItem;

								if( !currentReadItem->isNounValue() &&
								currentSpecificationWordTypeNr != WORD_TYPE_ADVERB &&
								currentReadItem->wordOrderNr() <= firstSpecificationWordOrderNr )
									{
									hasAddedSpecification = true;

									// Login is already created by during startup
									if( !generalizationWordItem->isVerbImperativeDisplayOrLoginOrRead() )
										{
										if( hasRelationWord )
											{
											// No selection, no value, with relation
											if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, NO_SELECTION_LEVEL, NO_LIST_NR, imperativeVerbParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											// No selection, no value, no relation
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a specification without relation to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}

										// Start reasoning
										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != NULL &&
										adminItem_->isNounWordType( currentSpecificationWordTypeNr ) )
											{
											// Generalizations with noun specifications - without relations
											if( !isPossessive )
												{
												if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, ( currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE ), questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( !commonVariables_->hasDisplayedWarning )
													{
													if( isGeneralizationProperName )
														{
														if( isSpanishCurrentLanguage &&
														!isExclusiveSpecification &&
														generalizationWordItem->isUserGeneralizationWord &&

														// Draw simple negative Spanish conclusions
														adminItem_->drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														// Definition specification
														if( ( compoundResult = adminItem_->drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperName &&
											!commonVariables_->hasDisplayedWarning &&
											commonVariables_->nUserRelationWords == 1 &&
											startRelationReadItem != NULL &&
											( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL &&
											( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL &&

											adminItem_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem->wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								}
							}
						}
					else
						{
						if( isPossessive &&
						currentReadItem->isArticle() )
							nContextRelations = 0;
						}

					if( !commonVariables_->hasDisplayedWarning &&
					!isQuestion &&
					currentSpecificationWordItem != NULL &&
					linkedSpecificationWordItem != NULL &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						if( isGeneralizationProperName &&
						!isValueSpecificationWord &&
						// Skip adjectives
						adminItem_->isNounWordType( linkedSpecificationWordTypeNr ) &&
						adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : NULL ) ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );

						if( isRelationWord ||

						( isSpecificationWord &&
						hasLinkedPossessiveDeterminer ) )
							{
							if( !commonVariables_->hasDisplayedWarning &&
							// Linked specification
							currentSpecificationWordItem == linkedSpecificationWordItem )
								{
								if( linkedGeneralizationWordItem_ != NULL )
									return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Linked word \"", linkedGeneralizationWordItem_->anyWordTypeString(), "\" is already assigned" );

								linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
								linkedGeneralizationWordItem_ = generalizationWordItem;
								}

							if( !commonVariables_->hasDisplayedWarning &&
							isGeneralizationProperName &&
							!isQuestion &&
							!isSpecificationGeneralization &&
							userRelationContextNr_ > NO_CONTEXT_NR &&
							commonVariables_->nUserRelationWords > 1 &&

							adminItem_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( !isSelection &&
							currentReadItem->isAdjectiveAssigned() &&
							inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem->readWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem->wordOrderNr(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		while( !commonVariables_->hasDisplayedWarning &&
		currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		if( !hasAddedSpecification )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperName &&
		!isQuestion &&
		!isSelection )
			{
			if( hasRelationWord )
				{
				if( recalculateAssumptionsInTouchedWords() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumptions of all touched words" );
				}
			else
				{
				if( generalizationWordItem->recalculateAssumptionsInWord() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isSpecificationWordSpanishAmbiguous;
		bool isExclusiveGeneralization = false;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *previousGeneralizationWordItem = NULL;
		WordItem *previousSpecificationWordItem = NULL;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationWordWithPreviousOne";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		if( specificationCollectionNr == NO_COLLECTION_NR )
			specificationCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr );

		if( ( currentGeneralizationWordItem = commonVariables_->firstSpecificationWordItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The first specification word is undefined" );

		// Do for all specification words
		do	{
			if( currentGeneralizationWordItem != generalizationWordItem &&
			currentGeneralizationWordItem->hasWordType( false, generalizationWordTypeNr ) &&
			// Try to find matching specification word
			( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, isPossessive, NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
				{
				// Relation word of a generalization word: proper name
				if( ( boolResult = currentGeneralizationWordItem->findGeneralization( true, generalizationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a generalization item" );

				// Generalization not found
				if( !boolResult.booleanValue )
					{
					// Languages and users
					if( foundSpecificationItem->isExclusiveGeneralization() ||

					( isAssignment &&
					!isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					foundSpecificationItem->isActiveAssignment() ) )
						isExclusiveGeneralization = true;

					previousGeneralizationWordItem = currentGeneralizationWordItem;
					previousSpecificationWordItem = foundSpecificationItem->specificationWordItem();
					}
				}
			}
		// Continue search to get the most recent generalization word
		while( ( currentGeneralizationWordItem = currentGeneralizationWordItem->nextSpecificationWordItem ) != NULL );

		if( previousGeneralizationWordItem != NULL &&
		collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalization words \"", previousGeneralizationWordItem->anyWordTypeString(), "\" and \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	SpecificationItem *simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	SpecificationItem *userSpecificationItem()
		{
		return userSpecificationItem_;
		}

	CreateAndAssignResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipDrawingSpanishAmbiguousSubstitutionConclusion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isUserGeneralizationWord;
		bool hasDisplayedIntegrityWarning = false;
		bool hasFoundRelationContext = false;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification = false;
		bool isAssumption = adminItem_->isAssumption( assumptionJustificationTypeNr );
		unsigned short justificationTypeNr = assumptionJustificationTypeNr;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *foundJustificationItem = NULL;
		JustificationItem *createdOrFoundJustificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != NULL &&
		primarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given primary specification item is hidden" );

		if( anotherPrimarySpecificationItem != NULL &&
		anotherPrimarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given another primary specification item is hidden" );

		if( secondarySpecificationItem != NULL &&
		secondarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given secondary specification item is hidden" );

		if( anotherSecondarySpecificationItem != NULL &&
		anotherSecondarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given another secondary specification item is hidden" );

		// Given assumption level has no value
		if( assumptionLevel == NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			assumptionLevel = adminItem_->assumptionGrade( ( anotherPrimarySpecificationItem != NULL ), hasFeminineOrMasculineProperNameEnding, ( primarySpecificationItem != NULL && primarySpecificationItem->isGeneralizationProperName() && primarySpecificationItem->isPossessive() ), ( primarySpecificationItem != NULL && primarySpecificationItem->isQuestion() ), assumptionJustificationTypeNr );

			if( primarySpecificationItem != NULL )
				assumptionLevel += primarySpecificationItem->assumptionLevel();

			if( anotherPrimarySpecificationItem != NULL )
				assumptionLevel += anotherPrimarySpecificationItem->assumptionLevel();

			if( secondarySpecificationItem != NULL )
				assumptionLevel += secondarySpecificationItem->assumptionLevel();

			if( anotherSecondarySpecificationItem != NULL )
				assumptionLevel += anotherSecondarySpecificationItem->assumptionLevel();

			// No assumption level: So, it is a conclusion
			if( assumptionLevel == NO_ASSUMPTION_LEVEL )
				{
				if( !adminItem_->isConclusion( conclusionJustificationTypeNr ) )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given conclusion justification type isn't a conclusion" );

				// Conclusion
				isAssumption = false;
				justificationTypeNr = conclusionJustificationTypeNr;
				}
			}

		if( ( justificationResult = generalizationWordItem->addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, false, justificationTypeNr, NO_ORDER_NR, commonVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a justification" );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, false, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to create a specification" );

		if( !commonVariables_->hasDisplayedWarning )
			{
			createdSpecificationItem = createAndAssignResult.createdSpecificationItem;

			if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
				hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

			if( createdSpecificationItem == NULL )
				{
				if( foundSpecificationItem == NULL )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem->isUserSpecification() )
					{
					hasDisplayedIntegrityWarning = true;
					sprintf( errorString, "\nI found a user specification, whilst trying to create a self-generated specification:\n\tSpecificationItem: %s;\n\tJustificationItem: %s.\n", foundSpecificationItem->itemToString( NO_WORD_TYPE_NR ), createdOrFoundJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}

				if( !hasDisplayedIntegrityWarning )
					{
					isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

					if( relationWordItem != NULL )
						hasRelationWordConfirmedSpecification = relationWordItem->hasConfirmedSpecification();

					// A justification has been created, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately
					if( ( createdJustificationItem == NULL &&

					( !foundSpecificationItem->hasJustification( false, foundJustificationItem ) ||

					( !isPossessive &&
					justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) ) ||

					( createdJustificationItem != NULL &&

					( !isAssumption ||
					foundSpecificationItem->isConcludedAssumption() ||
					foundSpecificationItem->isSelfGeneratedAssumption() ||

					// Conclusion becomes assumption
					( foundSpecificationItem->isSelfGeneratedConclusion() &&

					( justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||

					( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
					relationWordItem != NULL &&

					( ( isPossessive &&

					( ( !isUserGeneralizationWord &&
					!hasRelationWordConfirmedSpecification ) ||

					( isUserGeneralizationWord &&
					generalizationWordItem->hasConfirmedSpecification() &&
					foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) ||

					( !isPossessive &&
					!isUserGeneralizationWord &&
					hasRelationWordConfirmedSpecification &&
					!foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) ) ) ) ) )
						{
						if( processJustification( hasFoundRelationContext, hasRelationWord, hasRelationWordConfirmedSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to process the created justification" );
						}
					}
				}
			else
				{
				if( hasFoundRelationContext &&
				foundSpecificationItem != NULL &&
				!generalizationWordItem->hasConfirmedSpecification() &&
				foundSpecificationItem->relationContextNr() == createdSpecificationItem->relationContextNr() )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Integrity error! A specification was created in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" while a specification was found as well with the same relation context" );
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&
			adminItem_->isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != NULL &&

					// Draw a unique relation conclusion
					// Not yet explained in the design
					addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, createdSpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( ( createdJustificationItem != NULL ||
					createdSpecificationItem != NULL ) &&

					// Typically for the Spanish language
					!isSkipDrawingSpanishAmbiguousSubstitutionConclusion )
						{
						if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		createAndAssignResult.createdJustificationItem = createdJustificationItem;
		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
