﻿/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

// WordItem header

#ifndef WORDITEM
#define WORDITEM 1

#include "DuplicateResultType.cpp"
#include "GrammarResultType.cpp"
#include "JustificationResultType.cpp"
#include "RelatedResultType.cpp"
#include "WordResultType.cpp"

// Class declarations
class CollectionList;
class ContextItem;
class ContextList;
class GeneralizationItem;
class GeneralizationList;
class GrammarList;
class InterfaceList;
class JustificationList;
class MultipleWordList;
class SpecificationList;
class WordQuestion;
class WordSpecification;
class WordTypeList;
class WordWrite;
class WriteItem;
class WriteList;

class WordItem : protected Item
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class AdminReasoningNew;
	friend class AdminReasoningOld;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class CollectionItem;
	friend class CollectionList;
	friend class ContextItem;
	friend class ContextList;
	friend class GeneralizationItem;
	friend class GrammarItem;
	friend class GrammarList;
	friend class InputOutput;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class MultipleWordItem;
	friend class MultipleWordList;
	friend class ReadItem;
	friend class ReadList;
	friend class SelectionItem;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;

	// Private constructed variables

	bool isChineseLanguage_;
	bool isSpanishLanguage_;
	bool isNounWordSpanishAmbiguous_;

	unsigned int highestSentenceNrInWord_;

	char *authorizationKey_;

	// Word item supporting modules
	WordQuestion *wordQuestion_;
	WordSpecification *wordSpecification_;
	WordWrite *wordWrite_;

	// Word item lists
	SpecificationList *assignmentList_;
	CollectionList *collectionList_;
	ContextList *contextList_;
	GeneralizationList *generalizationList_;
	GrammarList *grammarList_;
	InterfaceList *interfaceList_;
	JustificationList *justificationList_;
	MultipleWordList *multipleWordList_;
	SpecificationList *specificationList_;
	WordTypeList *wordTypeList_;
	WriteList *writeList_;

	// Word item lists array
	List *wordListArray_[NUMBER_OF_WORD_LISTS];

	// This string is returned by a function. So, it must be "static".
	char wordItemNameString_[MAX_SENTENCE_STRING_LENGTH];


	// Private initialized variables

	bool isFemale_;
	bool isFeminineWord_;
	bool isMale_;
	bool isMasculineWord_;
	bool isLanguageWord_;

	unsigned short wordParameter_;


	// Private common functions

	unsigned short nUsers();

	char wordListChar( unsigned short wordListNr );

	char *selectedLanguageNameString( unsigned short languageNr );
	char *selectedUserNameString( unsigned short userNr );


	// Private error functions

	signed char addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );

	BoolResultType startBoolResultErrorInWord( const char *functionNameString, const char *errorString );

	CollectionResultType addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CollectionResultType startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	GrammarResultType startGrammarResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	JustificationResultType startJustificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordEndingResultType startWordEndingResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordTypeResultType startWordTypeResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );


	// Private assignment functions

	signed char inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char inactivateCurrentAssignments();

	SpecificationItem *firstActiveNonQuestionAssignmentItem( unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString );


	// Private collection functions

	unsigned short highestCollectionOrderNrInWord( unsigned int collectionNr );

	unsigned int highestCollectionNrInWord();


	// Private context functions

	bool hasContextInWord( unsigned int contextNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	bool isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr );


	// Private grammar functions

	void markGrammarAsChoiceEnd();
	void markGrammarAsOptionEnd();

	signed char checkForDuplicateGrammarDefinition();
	signed char checkGrammar();
	signed char linkLaterDefinedGrammarWords();
	signed char shrinkMergedWordsInWriteSentence();

	char *grammarString( unsigned short wordTypeNr );

	GrammarItem *firstReadingGrammarItem();
	GrammarItem *firstWritingGrammarItem( bool isQuestion );

	BoolResultType expandMergedWordsInReadSentence( char *readUserSentenceString );

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );

	WordEndingResultType analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );


	// Private interface functions

	signed char checkInterface( unsigned short interfaceParameter, char *interfaceString );
	signed char createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString_( unsigned short interfaceParameter );


	// Private specification functions

	bool isAuthorizedForChanges( char *authorizationKey );

	signed char checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem );


	// Private word type functions

	bool isSingularNounWord();


	protected:
	// Protected constructed variables

	bool isUserGeneralizationWord;
	bool isUserSpecificationWord;
	bool isUserRelationWord;

	bool isWordCheckedForSolving;

	unsigned short predefinedMultipleWordNr;

	WordItem *nextAssignmentWordItem;
	WordItem *nextCollectionWordItem;
	WordItem *nextContextWordItem;
	WordItem *nextPossessiveNounWordItem;
	WordItem *nextSpecificationWordItem;
	WordItem *nextTouchedWordItem;
	WordItem *nextUserProperNounWordItem;


	// Protected error functions

	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CreateAndAssignResultType startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType startCreateAndAssignResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	RelatedResultType addRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	RelatedResultType startRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	SpecificationResultType addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	SpecificationResultType startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );


	// Constructor

	WordItem();
	WordItem( bool isLanguageWord, unsigned short wordParameter, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList );
	~WordItem();


	// Protected virtual functions

	virtual void checkForUsage();

	virtual bool hasParameter( unsigned int queryParameter );
	virtual bool isSorted( Item *nextSortItem );

	virtual char *itemToString( unsigned short queryWordTypeNr );

	virtual BoolResultType findMatchingWordReferenceString( char *searchString );


	// Protected common functions

	void setCurrentLanguageAsChinese();
	void setHighestSentenceNr( unsigned int currentSentenceNr );

	bool isAdjectiveAssigned();
	bool isAdjectiveAssignedOrEmpty();
	bool isAdjectiveComparison();
	bool isAdjectiveComparisonEqual();
	bool isAdjectiveComparisonLess();
	bool isAdjectiveComparisonMore();
	bool isAdjectiveEven();
	bool isAdjectiveOdd();
	bool isAdjectiveOddOrEven();

	bool isAdminWord();
	bool isAuthorizationRequiredForChanges();
	bool isChineseCurrentLanguage();
	bool isSpanishCurrentLanguage();
	bool isFemale();
	bool isMale();
	bool isFemaleOrMale();

	bool isChineseReversedImperativeNoun();
	bool isNounHead();
	bool isNounTail();
	bool isNounNumber();
	bool isNounPassword();
	bool isNounValue();

	bool isBasicVerb();
	bool isImperativeVerbDisplay();
	bool isImperativeVerbDisplayLoginOrRead();
	bool isImperativeVerbUndoOrRedo();

	unsigned short userNr( WordItem *userWordItem );
	unsigned short wordParameter();

	unsigned int highestSentenceNrInWord();

	signed char assignChangePermissions( char *authorizationKey );
	signed char checkStructuralIntegrityInWord();
	signed char markWordAsFemale();
	signed char markWordAsFeminine();
	signed char markWordAsMale();
	signed char markWordAsMasculine();

	char *currentLanguageNameString();
	char *languageNameString( unsigned short languageNr );
	char *userNameString( unsigned short userNr );
	char *wordTypeNameString( unsigned short wordTypeNr );

	WordItem *languageWordItem( unsigned short languageNr );
	WordItem *nextWordItem();
	WordItem *predefinedWordItem( unsigned short wordParameter );


	// Protected assignment functions

	unsigned int nActiveAssignments();

	signed char archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem );
	signed char createNewAssignmentLevelInWord();
	signed char deleteAssignmentLevelInWord();
	signed char inactivateActiveAssignment( SpecificationItem *activeAssignmentItem );

	SpecificationItem *firstActiveNumeralAssignmentItem();
	SpecificationItem *firstActiveStringAssignmentItem();
	SpecificationItem *lastActiveNonQuestionAssignmentItem();

	SpecificationItem *firstAssignmentItem( bool isPossessive, bool isQuestion, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstNonPossessiveActiveAssignmentItem( WordItem *relationWordItem );
	SpecificationItem *firstNonQuestionActiveAssignmentItem();
	SpecificationItem *firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem );
	SpecificationItem *firstQuestionAssignmentItem();

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *authorizationKey );


	// Protected cleanup functions

	void clearReplacingInfoInWord();
	void rebuildQuickAccessWordLists();

	unsigned int highestCurrentSentenceItemNrInWord( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr );
	unsigned int highestFoundSentenceNrInWord( bool isIncludingTemporaryLists, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr );

	signed char decrementItemNrRangeInWord( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset );
	signed char decrementSentenceNrsInWord( unsigned int startSentenceNr );
	signed char deleteSentencesInWord( unsigned int lowestSentenceNr );
	signed char redoCurrentSentenceInWord();
	signed char removeFirstRangeOfDeletedItemsInWord();
	signed char undoCurrentSentenceInWord();


	// Protected collection functions

	bool hasCollection();
	bool hasCollectionNr( unsigned int collectionNr );
	bool hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem );
	bool hasNonExclusiveCollection( unsigned int collectionNr );

	bool isCollectionSpanishAmbiguous( unsigned int collectionNr );
	bool isCompoundCollection( unsigned int collectionNr );
	bool isNonCompoundCollection( unsigned int collectionNr );
	bool isNounWordSpanishAmbiguous();

	unsigned short highestCollectionOrderNrInCollectionWords( unsigned int collectionNr );

	unsigned int collectionNr( unsigned short collectionWordTypeNr );
	unsigned int collectionNr( WordItem *commonWordItem );
	unsigned int collectionNrByCompoundGeneralizationWordInWord( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem );

	unsigned int compoundCollectionNr();
	unsigned int nonCompoundCollectionNr();
	unsigned int nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr );
	unsigned int highestCollectionNrInCollectionWords();

	WordItem *collectionWordItem( unsigned int compoundCollectionNr );
	WordItem *commonWordItem( unsigned int collectionNr );
	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr );
	WordItem *feminineCollectionWordItem();
	WordItem *masculineCollectionWordItem();

	BoolResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem );

	CollectionResultType createCollection( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem );


	// Protected context functions

	bool hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr );
	bool hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool hasFoundContextInAllWords( unsigned int contextNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	bool isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isContextSubsetInContextWords( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	unsigned short contextWordTypeNr( unsigned int contextNr );

	unsigned int contextNr( WordItem *specificationWordItem );
	unsigned int contextNr( unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );
	unsigned int highestContextNrInWord();
	unsigned int nContextWords( unsigned int contextNr, WordItem *specificationWordItem );

	signed char addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );

	ContextItem *firstActiveContextItem();
	ContextItem *contextItem( unsigned int contextNr );
	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );


	// Protected database connection functions

//	signed char storeChangesInFutureDatabaseInWord();


	// Protected generalization functions

	signed char createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem );

	GeneralizationItem *firstGeneralizationItem();
	GeneralizationItem *firstNounSpecificationGeneralizationItem();
	GeneralizationItem *firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage );
	GeneralizationItem *firstRelationGeneralizationItem();

	BoolResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem );


	// Protected grammar functions

	void checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem );
	void markGrammarOfCurrentLanguageAsChoiceEnd();
	void markGrammarOfCurrentLanguageAsOptionEnd();

	bool isLanguageWord();

	unsigned short nLanguages();

	signed char checkForDuplicateGrammarDefinitionInCurrentLanguage();
	signed char checkGrammarOfCurrentLanguage();
	signed char linkLaterDefinedGrammarWordsInCurrentLanguage();
	signed char shrinkMergedWordsInWriteSentenceOfCurrentLanguage();

	GrammarItem *firstCurrentLanguageReadingGrammarItem();
	GrammarItem *firstCurrentLanguageWritingGrammarItem( bool isQuestion );

	GrammarResultType createGrammarItemForCurrentLanguage( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammarOfCurrentLanguage( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );

	BoolResultType expandMergedWordsInReadSentenceOfCurrentLanguage( char *readUserSentenceString );
	WordEndingResultType analyzeWordEndingWithCurrentLanguage( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );


	// Protected interface functions

	signed char checkInterfaceOfCurrentLanguage( unsigned short interfaceParameter, char *interfaceString );
	signed char createInterfaceForCurrentLanguage( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString( unsigned short interfaceParameter );


	// Protected justification functions

	void checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem );
	void clearJustificationHasBeenWritten();

	bool hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();

	signed char attachJustificationInWord( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem );
	signed char replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem );
	signed char replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem );
	signed char replaceOrDeleteUnreferencedJustifications();
	signed char updateSpecificationsInJustificationInWord( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr );

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNounEnding, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem );
	JustificationResultType copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem );

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem();
	JustificationItem *negativeAssumptionOrConclusionJustificationItem( SpecificationItem *secondarySpecificationItem );
	JustificationItem *olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, bool isPossessiveSecondarySpecification, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *possessiveReversibleAssumptionJustificationItem( WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem );
	JustificationItem *primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem );

	SpecificationItem *suggestiveQuestionAssumptionSecondarySpecificationItem();


	// Protected multiple word functions

	bool isMultipleWord();

	unsigned short matchingMultipleWordParts( char *sentenceString );

	signed char addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem );


	// Protected query functions

	void countQuery();
	void clearQuerySelections();

	void itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	void listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString );
	void parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter );
	void wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr );

	bool hasActiveQuestionWithCompoundCollection();

	signed char displayQueryResultInWord( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	signed char stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString );
	signed char wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordReferenceNameString );
	signed char wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );


	// Protected question functions

	bool hasCurrentlyAnsweredSelfGeneratedQuestion();
	bool hasFoundAnswerToQuestion();

	signed char findAnswerToNewUserQuestion();
	signed char findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem );
	signed char writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem );


	// Protected specification functions

	void checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem );
	void initializeVariablesInWord();

	bool hadOnceAnsweredSelfGeneratedQuestion();
	bool hasAnyUserSpecification();
	bool hasCurrentlyConfirmedSpecification();
	bool hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
	bool hasCurrentlyConfirmedSpecificationButNoRelation();
	bool hasCorrectedAssumption();
	bool hasCorrectedAssumptionByKnowledge();
	bool hasCorrectedAssumptionByOppositeQuestion();
	bool hasDisplayedMoreSpecificSpecification();
	bool hasDisplayedMoreSpecificNonExclusiveSpecification();
	bool hasDisplayedMoreSpecificQuestion();
	bool hasMultipleSpecificationWordsWithSameSentenceNr( unsigned int creationSentenceNr, unsigned int notThisItemNr, unsigned int specificationCollectionNr );
	bool hasPartOfSpecification();
	bool hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );
	bool hasRelationContextInSpecificationsInWord( unsigned int relationContextNr );

	bool isJustificationInUse( JustificationItem *unusedJustificationItem );

	unsigned int nRemainingSpecificationWordsForWriting( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, unsigned int creationSentenceNr );

	signed char changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem );
	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char clearStoredSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem );
	signed char collectGeneralizations( bool isExclusiveGeneralization, unsigned int generalizationCollectionNr );
	signed char collectSpecificationsInWord( bool isExclusiveSpecification, bool isQuestion, unsigned int specificationCollectionNr );
	signed char confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem );
	signed char copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem );
	signed char recalculateAssumptionsInWord();
	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem );
	signed char updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem );

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingAnsweredQuestions, bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyGeneralizationContext, bool isAllowingEmptyRelationContext, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstActiveQuestionSpecificationItem();
	SpecificationItem *firstAssignmentOrSpecificationItem( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem );

	SpecificationItem *firstExclusiveSpecificationItem();
	SpecificationItem *firstExclusiveSpecificationItem( WordItem *specificationWordItem );
	SpecificationItem *firstFeminineOrMasculineSpecificationItem();

	SpecificationItem *firstNonCompoundUserSpecificationItem();
	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int specificationCollectionNr );
	SpecificationItem *firstNonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives );
	SpecificationItem *firstNonQuestionSpecificationItem();

	SpecificationItem *firstPossessiveSpecificationItem();
	SpecificationItem *firstPrimarySpecificationCandidateOfQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification );
	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem();
	SpecificationItem *firstRelationSpecificationItem();
	SpecificationItem *firstSelectedRelationSpecificationItem( bool isAssignment, bool isArchivedAssignment );

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter );
	SpecificationItem *firstSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem );
	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstUnhiddenSpanishSpecificationItem();
	SpecificationItem *firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *noRelationContextSpecificationItem( bool isPossessive, bool isSelfGenerated, WordItem *specificationWordItem );
	SpecificationItem *partOfSpecificationItem( WordItem *specificationWordItem );
	SpecificationItem *sameUserQuestionSpecificationItem( unsigned short questionParameter );

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord );

	BoolResultType findQuestionToBeAdjustedByCompoundCollection( unsigned int questionSpecificationCollectionNr, WordItem *primarySpecificationWordItem );

	CollectionResultType collectSpecifications( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem );

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey );
	CreateAndAssignResultType copySpecificationItem( unsigned int specificationCollectionNr, JustificationItem *firstJustificationItem, SpecificationItem *originalSpecificationItem );
	CreateAndAssignResultType createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString );

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem );
	RelatedResultType findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );


	// Protected word type functions

	void clearGeneralizationWriteLevel( unsigned short currentWriteLevel );
	void clearSpecificationWriteLevel( unsigned short currentWriteLevel );
	void clearRelationWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr );

	bool hasAnyWordType();
	bool hasFeminineAndMasculineArticle( unsigned short articleParameter );
	bool hasFeminineProperNounEnding();
	bool hasFeminineOrMasculineProperNounEnding();
	bool hasMasculineProperNounEnding();
	bool hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr );

	bool isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr );
	bool isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr );

	bool isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr );
	bool isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *authorizationKey );
	bool isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr );
	bool isNumeralWordType();
	bool isProperNounPrecededByDefiniteArticle( unsigned short definiteArticleParameter );
	bool isProperNounWord();

	signed char deleteAllWordTypesOfCurrentSentence();
	signed char deleteWordType( WordTypeItem *deleteWordTypeItem );
	signed char hideWordType( unsigned short wordTypeNr, char *authorizationKey );

	signed char markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr );
	signed char markSpecificationWordTypeAsWritten( unsigned short wordTypeNr );
	signed char markRelationWordTypeAsWritten( unsigned short wordTypeNr );

	WordTypeItem *activeWordTypeItem( unsigned short wordTypeNr );
	WordTypeItem *activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	char *activeWordTypeString( unsigned short wordTypeNr );
	char *anyWordTypeString();
	char *singularNounString();
	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	WordTypeResultType addWordType( bool isMultipleWord, bool isProperNounPrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString );
	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString );


	// Protected write functions

	void deleteTemporaryWriteList();

	signed char createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem );
	signed char writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem );
	signed char writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );
	signed char writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecification( bool isAdjustedAssumption, bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isJustification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, WordItem *writeWordItem );
	signed char writeSpecificationSentence( bool isAssignment, bool isArchivedAssignment, bool isCheckingUserSentenceForIntegrity, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem );
	signed char writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, bool isReplacedBySpecificationWithRelation, bool wasHiddenSpanishSpecification, SpecificationItem *writeSpecificationItem );

	WriteItem *firstActiveWriteItem();
	};
#endif

/*************************************************************************
 *	"Those who look to him for help will be radiant with joy;
 *	no shadow of shame wil darken their faces." (Psalm 34:5)
 *************************************************************************/
