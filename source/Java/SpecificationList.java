/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
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

class SpecificationList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsSpecificationWord_;


	// Private assignment methods

	private byte createNewAssignmentLevel( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( searchSpecificationItem != null &&
		searchSpecificationItem.assignmentLevel() > CommonVariables.currentAssignmentLevel )
			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithCurrentLevel();

		while( searchSpecificationItem != null )
			{
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchSpecificationItem.isAnsweredQuestion(), searchSpecificationItem.isConcludedAssumption(), searchSpecificationItem.isConditional(), searchSpecificationItem.isCorrectedAssumption(), searchSpecificationItem.isEveryGeneralization(), searchSpecificationItem.isExclusiveGeneralization(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPartOf(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.isValueSpecification(), (short)(searchSpecificationItem.assignmentLevel() + 1 ), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.languageNr(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.generalizationWordTypeNr(), searchSpecificationItem.specificationWordTypeNr(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationCollectionNr(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.originalSentenceNr(), searchSpecificationItem.activeSentenceNr(), searchSpecificationItem.inactiveSentenceNr(), searchSpecificationItem.archivedSentenceNr(), searchSpecificationItem.nContextRelations(), searchSpecificationItem.firstJustificationItem(), searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), searchSpecificationItem.lastWrittenSentenceStringBuffer ).result != Constants.RESULT_OK )
				return addError( 1, "I failed to copy an assignment item with incremented assignment level" );

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithCurrentLevel();
			}

		return Constants.RESULT_OK;
		}

	private byte deleteAssignmentLevelInList( SpecificationItem searchSpecificationItem )
		{
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;

		while( searchSpecificationItem != null &&
		searchSpecificationItem.assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchSpecificationItem.assignmentLevel() == currentAssignmentLevel )
				{
				if( deleteItem( searchSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to delete an item" );

				searchSpecificationItem = nextSpecificationListItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte removeJustificationsFromConfirmedAssumptionOrConclusion( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean isUserSpecification;
		JustificationItem attachedJustificationItem;
		JustificationItem currentJustificationItem;
		WordItem generalizationWordItem = myWordItem();

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem == null )
			return startError( 1, "The given replacing specification item is undefined" );

		isUserSpecification = replacingSpecificationItem.isUserSpecification();

		if( ( currentJustificationItem = obsoleteSpecificationItem.firstJustificationItem() ) == null )
			return startError( 1, "The given obsolete specification item has no justifications" );

		do	{
			// Remember next justification
			attachedJustificationItem = currentJustificationItem.attachedJustificationItem();

			if( currentJustificationItem.isOlderItem() &&
			!generalizationWordItem.isJustificationInUse( currentJustificationItem ) )
				{
				if( isUserSpecification )
					{
					// Remove justification from confirmed assumption/conclusion
					if( generalizationWordItem.replaceJustification( currentJustificationItem, null, obsoleteSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to remove a justification from a confirmed specification" );
					}
				else
					{
					// Self-generated question or corrected specification
					if( currentJustificationItem.isActiveItem() &&
					!generalizationWordItem.isJustificationInUse( currentJustificationItem ) &&
					generalizationWordItem.replaceOrDeleteJustification( currentJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace or delete a justification" );
					}
				}
			}
		while( ( currentJustificationItem = attachedJustificationItem ) != null );

		return Constants.RESULT_OK;
		}

	private byte updateReplacedSpecifications( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		SpecificationItem searchSpecificationItem = firstReplacedSpecificationItem();

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isReplacedItem() )
			return startError( 1, "The given replaced specification item is a replaced item" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.replacingSpecificationItem == obsoleteSpecificationItem )
				searchSpecificationItem.replacingSpecificationItem = replacingSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}


	// Private specification methods

	private static void clearReplacingInfo( SpecificationItem searchSpecificationItem )
		{
		while( searchSpecificationItem != null )
			{
			searchSpecificationItem.clearReplacingInfo();
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}
		}

	private byte confirmSpecificationInJustification( boolean isConfirmedPrimarySpecification, JustificationItem obsoleteJustificationItem, SpecificationItem confirmationSpecificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		JustificationItem createdJustificationItem;
		WordItem generalizationWordItem = myWordItem();

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( confirmationSpecificationItem == null )
			return startError( 1, "The given confirmation specification item is undefined" );

		if( !confirmationSpecificationItem.isUserSpecification() )
			return startError( 1, "The given confirmation specification item isn't a user specification" );

		if( ( justificationResult = generalizationWordItem.copyJustification( false, ( isConfirmedPrimarySpecification ? confirmationSpecificationItem : obsoleteJustificationItem.primarySpecificationItem() ), ( isConfirmedPrimarySpecification ? obsoleteJustificationItem.secondarySpecificationItem() : confirmationSpecificationItem ), obsoleteJustificationItem.attachedJustificationItem(), obsoleteJustificationItem ) ).result != Constants.RESULT_OK )
			return addError( 1, "I failed to copy the primary specification Justification item with a different primary specification item" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
			return startError( 1, "I couldn't create a justification item" );

		if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, confirmationSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to replace the given obsolete justification item by a created justification item" );

		return Constants.RESULT_OK;
		}

	private byte updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem generalizationWordItem = myWordItem();

		// Update specifications in justifications of this word
		if( generalizationWordItem.updateSpecificationsInJustificationInWord( true, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the specifications in my justification items" );

		if( ( currentGeneralizationItem = generalizationWordItem.firstGeneralizationItem() ) != null )
			{
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return startError( 1, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem.updateSpecificationsInJustificationInWord( false, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to update the specifications in the justification items in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private SpecificationItem firstActiveSpecificationItem()
		{
		return (SpecificationItem)firstActiveItem();
		}

	private SpecificationItem firstInactiveSpecificationItem()
		{
		return (SpecificationItem)firstInactiveItem();
		}

	private SpecificationItem firstArchivedSpecificationItem()
		{
		return (SpecificationItem)firstArchivedItem();
		}

	private SpecificationItem firstReplacedSpecificationItem()
		{
		return (SpecificationItem)firstReplacedItem();
		}

	private SpecificationItem firstSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		return ( isArchivedAssignment ? firstArchivedSpecificationItem() : ( isInactiveAssignment ? firstInactiveSpecificationItem() : firstActiveSpecificationItem() ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, isQuestion ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	private SpecificationItem nextSpecificationListItem()
		{
		return (SpecificationItem)nextListItem();
		}


	// Constructor

	protected SpecificationList( char _listChar, WordItem myWordItem )
		{
		// Private constructed variables

		isMarkedAsSpecificationWord_ = false;

		initializeListVariables( _listChar, "SpecificationList", myWordItem );
		}


	// Protected assignment methods

	protected int nActiveAssignments()
		{
		int nItems = 0;
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != null )
			{
			nItems++;
			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return nItems;
		}

	protected byte createNewAssignmentLevelInList()
		{
		if( CommonVariables.currentAssignmentLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, "Assignment level overflow" );

		if( createNewAssignmentLevel( false, false ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an active assignment level" );

		if( createNewAssignmentLevel( true, false ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an inactive assignment level" );

		if( createNewAssignmentLevel( false, true ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an archive assignment level" );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInList()
		{
		SpecificationItem searchSpecificationItem;

		if( CommonVariables.currentAssignmentLevel <= Constants.NO_ASSIGNMENT_LEVEL )
			return startError( 1, "The current assignment level is undefined" );

		if( deleteAssignmentLevelInList( firstActiveSpecificationItem() ) == Constants.RESULT_OK )
			{
			if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != null )
				deleteAssignmentLevelInList( searchSpecificationItem );

			if( CommonVariables.result == Constants.RESULT_OK &&
			( searchSpecificationItem = firstArchivedSpecificationItem() ) != null )
				deleteAssignmentLevelInList( searchSpecificationItem );
			}

		return CommonVariables.result;
		}

	protected byte inactivateOrArchiveCurrentGeneralizationAssignments( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundActiveAssignmentItem = null;
		SpecificationItem foundInactiveAssignmentItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem foundActiveGeneralizationWordItem;
		WordItem foundInactiveGeneralizationWordItem;

		if( specificationWordItem == null )
			return startError( 1, "The given specification word item is undefined" );

		if( specificationWordItem.isNounValue() )
			return startError( 1, "Inactivating or archiving a noun value specification isn't implemented yet" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return startError( 1, "I found an undefined generalization word" );

				if( foundActiveAssignmentItem == null &&
				// Skip my word for activate assignments, because this is the one to be assigned
				currentGeneralizationWordItem != myWordItem() )
					foundActiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( true, false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

				// Allow to find an inactive assignment in my word
				if( foundInactiveAssignmentItem == null )
					foundInactiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( false, true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );
				}
			while( ( foundActiveAssignmentItem == null ||
			foundInactiveAssignmentItem == null ) &&

			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		// Only archive an inactive assignment if an active assignment is being inactivated
		if( foundActiveAssignmentItem != null )
			{
			if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem.generalizationWordItem() ) == null )
				return startError( 1, "I found an active assignment without generalization word item" );

			if( foundActiveGeneralizationWordItem.inactivateActiveAssignment( foundActiveAssignmentItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to inactivate an active assignment" );

			if( foundInactiveAssignmentItem != null )
				{
				if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem.generalizationWordItem() ) == null )
					return startError( 1, "I found an inactive assignment without generalization word item" );

				if( foundInactiveGeneralizationWordItem.archiveInactiveAssignment( foundInactiveAssignmentItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to archive an inactive assignment" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isSpecificationNumeral() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationString() != null )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstActiveNonQuestionAssignmentItem( boolean isDifferentRelationContext, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&

			( ( !isDifferentRelationContext &&
			searchSpecificationItem.relationContextNr() == relationContextNr ) ||

			( isDifferentRelationContext &&
			searchSpecificationItem.hasRelationContext() &&
			searchSpecificationItem.relationContextNr() != relationContextNr ) ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstAnsweredQuestionAssignmentItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( true, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isAnsweredQuestion() &&
			searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.relationContextNr() == relationContextNr )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( true );
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		// This is the first assignment
		SpecificationItem firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		// Now get the first valid assignment
		return ( firstAssignmentItem == null ? null : firstAssignmentItem.assignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) );
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		SpecificationItem firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		return ( firstAssignmentItem == null ? null : firstAssignmentItem.assignmentItem( isIncludingAnsweredQuestions, true, questionParameter ) );
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.relationContextNr() == relationContextNr )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem lastActiveNonQuestionAssignmentItem()
		{
		SpecificationItem lastAssignmentItem = null;
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			lastAssignmentItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return lastAssignmentItem;
		}


	// Protected specification methods

	protected void addToAssignmentWordQuickAccessList()
		{
		// Word order is not important
		myWordItem().nextAssignmentWordItem = CommonVariables.firstAssignmentWordItem;
		CommonVariables.firstAssignmentWordItem = myWordItem();
		}

	protected void addToPossessiveNounWordQuickAccessList()
		{
		// Word order is not important
		myWordItem().nextPossessiveNounWordItem = CommonVariables.firstPossessiveNounWordItem;
		CommonVariables.firstPossessiveNounWordItem = myWordItem();
		}

	protected void addToSpecificationWordQuickAccessList()
		{
		// Word order is not important
		myWordItem().nextSpecificationWordItem = CommonVariables.firstSpecificationWordItem;
		CommonVariables.firstSpecificationWordItem = myWordItem();
		}

	protected void clearReplacingInfo()
		{
		SpecificationItem searchSpecificationItem;

		if( ( searchSpecificationItem = firstActiveSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstArchivedSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstReplacedSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );
		}

	protected void initializeSpecificationVariables( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != null )
			{
			searchSpecificationItem.initializeSpecificationVariables();
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, true );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasSpecificationCompoundCollection() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasAnsweredSelfGeneratedQuestion()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isAnsweredSelfGeneratedQuestion() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return false;
		}

	protected boolean hasAnyUserSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.userNr() > Constants.NO_USER_NR )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasContextInSpecifications( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedSpecification, int contextNr )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedSpecification ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.generalizationContextNr() == contextNr ||
				searchSpecificationItem.specificationContextNr() == contextNr ||
				searchSpecificationItem.relationContextNr() == contextNr )
					return true;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected boolean hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPartOf() &&
			!searchSpecificationItem.isPossessive() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasPossessiveButNonQuestionSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasPossiblyGapInKnowledge( int exclusiveSecondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		int primarySpecificationCollectionNr;
		int searchSpecificationCollectionNr;
		SpecificationItem searchSpecificationItem = firstSpecificationItem( false, false );

		if( primarySpecificationItem != null )
			{
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem != primarySpecificationItem &&
				!searchSpecificationItem.isPossessive() &&
				!searchSpecificationItem.isPartOf() )
					{
					searchSpecificationCollectionNr = searchSpecificationItem.specificationCollectionNr();

					// Don't skip creation of current question
					if( ( searchSpecificationCollectionNr == primarySpecificationCollectionNr ||
					searchSpecificationCollectionNr == exclusiveSecondarySpecificationCollectionNr ) &&

					( searchSpecificationItem.isNegative() ||
					searchSpecificationItem.isOlderItem() ) )
						return false;
					}

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return true;
		}

	protected boolean isJustificationInUse( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedJustificationItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.hasJustification( !isReplacedItem, unusedJustificationItem ) )
					return true;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected byte changeJustificationOfNegativeAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem secondarySpecificationItem )
		{
		JustificationResultType justificationResult;
		int specificationCollectionNr;
		JustificationItem firstJustificationItem;
		JustificationItem foundOrCreatedJustificationItem;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem generalizationWordItem = myWordItem();

		if( secondarySpecificationItem == null )
			return startError( 1, "The given secondary specification item is undefined" );

		specificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() &&
			searchSpecificationItem.isSelfGeneratedAssumption() &&
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
			( firstJustificationItem = searchSpecificationItem.firstJustificationItem() ) != null )
				{
				if( firstJustificationItem.isOlderItem() )
					{
					if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, firstJustificationItem.justificationTypeNr(), firstJustificationItem.orderNr, firstJustificationItem.originalSentenceNr(), firstJustificationItem.primarySpecificationItem(), firstJustificationItem.anotherPrimarySpecificationItem(), secondarySpecificationItem, null, firstJustificationItem.attachedJustificationItem() ) ).result != Constants.RESULT_OK )
						return addError( 1, "I failed to add a justification item" );

					foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == null ? justificationResult.foundJustificationItem : justificationResult.createdJustificationItem );

					if( firstJustificationItem != foundOrCreatedJustificationItem )
						{
						if( generalizationWordItem.replaceJustification( firstJustificationItem, foundOrCreatedJustificationItem, searchSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to replace a justification of a negative assumption" );

						searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
						}
					else
						searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
					}
				else
					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != null &&
		searchSpecificationItem.checkForDeletedJustification() == Constants.RESULT_OK )
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();

		return CommonVariables.result;
		}

	protected byte checkForReplacedOrDeletedJustification( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != null &&
		searchSpecificationItem.checkForReplacedOrDeletedJustification() == Constants.RESULT_OK )
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();

		return CommonVariables.result;
		}

	protected byte checkJustificationForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedJustificationItem == null )
			return startError( 1, "The given unused justification item is undefined" );

		while( searchSpecificationItem != null &&
		!CommonVariables.hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem.hasJustification( !isReplacedItem, unusedJustificationItem ) &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a justification item that is still in use:\n\tSpecificationItem: " + searchSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + unusedJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, SpecificationItem unusedSpecificationItem )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedSpecificationItem == null )
			return startError( 1, "The given unused specification item is undefined" );

		while( searchSpecificationItem != null &&
		!CommonVariables.hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem.replacingSpecificationItem == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replacing specification item that is still in use:\n\tSpecificationItem: " + searchSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\treplacingSpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, WordItem unusedWordItem )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationWordItem() == unusedWordItem )
				return startError( 1, "The specification word item is still in use" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte clearLastWrittenSentenceStringWithUnknownPluralNoun( boolean isInactiveAssignment, boolean isArchivedAssignment, String unknownPluralNounString, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unknownPluralNounString == null )
			return startError( 1, "The given unknown plural noun string is undefined" );

		if( specificationWordItem == null )
			return startError( 1, "The given specification word item is undefined" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.lastWrittenSentenceStringBuffer != null &&
			searchSpecificationItem.lastWrittenSentenceStringBuffer.indexOf( unknownPluralNounString ) > 0 )
				searchSpecificationItem.lastWrittenSentenceStringBuffer = null;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizations( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem generalizationWordItem = myWordItem();

		if( generalizationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, "The given generalization collection number is undefined" );

		while( searchSpecificationItem != null )
			{
			if( ( isExclusiveGeneralization ||
			searchSpecificationItem.isGeneralizationNoun() ) &&

			!searchSpecificationItem.hasGeneralizationCollection() &&
			generalizationWordItem.hasCollectionNr( generalizationCollectionNr, searchSpecificationItem.specificationWordItem() ) )
				{
				if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem.collectGeneralization( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
						return addError( 1, "I failed to collect the generalization of a specification" );

					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
					}
				else
					{
					if( copyAndReplaceSpecificationItem( searchSpecificationItem.isAnsweredQuestion(), isExclusiveGeneralization, searchSpecificationItem.isExclusiveSpecification(), generalizationCollectionNr, searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.firstJustificationItem(), searchSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte collectSpecificationsInList( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isQuestion, int specificationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem specificationWordItem;

		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, "The given specification collection number is undefined" );

		while( searchSpecificationItem != null )
			{
			specificationWordItem = searchSpecificationItem.specificationWordItem();

			if( specificationWordItem != null &&
			!searchSpecificationItem.hasSpecificationCollection() &&
			specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
				{
				if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem.collectSpecification( isExclusiveSpecification, specificationCollectionNr ) != Constants.RESULT_OK )
						return addError( 1, "I failed to collect a specification" );

					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
					}
				else
					{
					if( copyAndReplaceSpecificationItem( searchSpecificationItem.isAnsweredQuestion(), false, isExclusiveSpecification, searchSpecificationItem.generalizationCollectionNr(), specificationCollectionNr, searchSpecificationItem.firstJustificationItem(), searchSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte confirmSpecificationButNotItsRelation( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem confirmedSpecificationItem, SpecificationItem confirmationSpecificationItem )
		{
		JustificationItem foundJustificationItem;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		if( confirmedSpecificationItem == null )
			return startError( 1, "The given confirmed specification but not its relation(s) specification item is undefined" );

		if( !confirmedSpecificationItem.isSelfGenerated() )
			return startError( 1, "The given confirmed specification but not its relation(s) specification item isn't self-generated" );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.hasRelationContext() &&
			( foundJustificationItem = searchSpecificationItem.primaryOrSecondarySpecificationJustificationItem( confirmedSpecificationItem ) ) != null )
				{
				if( confirmSpecificationInJustification( ( foundJustificationItem.primarySpecificationItem() == confirmedSpecificationItem ), foundJustificationItem, confirmationSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to confirm a specification in the found justification" );

				searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte copyAndReplaceSpecificationItem( boolean isNewAnsweredQuestion, boolean isNewExclusiveGeneralization, boolean isNewExclusiveSpecification, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		SpecificationItem createdSpecificationItem = null;
		CreateAndAssignResultType createAndAssignResult;

		if( originalSpecificationItem == null )
			return startError( 1, "The given original specification item is undefined" );

		if( ( createAndAssignResult = createSpecificationItem( originalSpecificationItem.isInactiveAssignment(), originalSpecificationItem.isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem.isConcludedAssumption(), originalSpecificationItem.isConditional(), originalSpecificationItem.isCorrectedAssumption(), originalSpecificationItem.isEveryGeneralization(), isNewExclusiveGeneralization, isNewExclusiveSpecification, originalSpecificationItem.isNegative(), originalSpecificationItem.isPartOf(), originalSpecificationItem.isPossessive(), originalSpecificationItem.isSpecificationGeneralization(), originalSpecificationItem.isUniqueUserRelation(), originalSpecificationItem.isValueSpecification(), originalSpecificationItem.assignmentLevel(), originalSpecificationItem.assumptionLevel(), originalSpecificationItem.languageNr(), originalSpecificationItem.prepositionParameter(), originalSpecificationItem.questionParameter(), originalSpecificationItem.generalizationWordTypeNr(), originalSpecificationItem.specificationWordTypeNr(), originalSpecificationItem.relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem.generalizationContextNr(), originalSpecificationItem.specificationContextNr(), originalSpecificationItem.relationContextNr(), originalSpecificationItem.originalSentenceNr(), originalSpecificationItem.activeSentenceNr(), originalSpecificationItem.inactiveSentenceNr(), originalSpecificationItem.archivedSentenceNr(), originalSpecificationItem.nContextRelations(), newFirstJustificationItem, originalSpecificationItem.specificationWordItem(), originalSpecificationItem.specificationString(), originalSpecificationItem.lastWrittenSentenceStringBuffer ) ).result != Constants.RESULT_OK )
			return addError( 1, "I failed to create a specification item with different parameters" );

		if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return startError( 1, "I couldn't create a specification item" );

		if( replaceOrDeleteSpecification( false, originalSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to replace or delete the given specification item" );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isSelfGeneratedAssumption() &&
			// Don't calculate hidden Spanish assumptions
			!searchSpecificationItem.isHiddenSpanishSpecification() &&
			searchSpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
				return addError( 1, "I failed to recalculate an assumption" );

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteSpecification( boolean isAllowingReplacedObsoleteSpecification, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean isAssignment = isAssignmentList();
		JustificationItem obsoleteJustificationItem;

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		if( !isAllowingReplacedObsoleteSpecification &&
		obsoleteSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, "The given obsolete specification item is already replaced or deleted" );

		if( obsoleteSpecificationItem == replacingSpecificationItem )
			return startError( 1, "The given obsolete specification item and the given replacing specification item are the same specification item" );

		if( obsoleteSpecificationItem.isAssignment() != isAssignment )
			return startError( 1, "The given obsolete specification item is an assignment item and I am a specification list, or the given obsolete specification item is a specification item and I am an assignment list" );

		if( !isAllowingReplacedObsoleteSpecification &&
		replacingSpecificationItem != null &&
		replacingSpecificationItem.isAssignment() != isAssignment )
			return startError( 1, "The given replacing specification item is an assignment item and I am a specification list, or the given replacing specification item is a specification item and I am an assignment list" );

		// Don't replace a normal specification by a hidden specification
		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isHiddenSpanishSpecification() &&
		!obsoleteSpecificationItem.isHiddenSpanishSpecification() &&
		obsoleteSpecificationItem.specificationCollectionNr() == replacingSpecificationItem.specificationCollectionNr() )
			return startError( 1, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );

		if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the replacing specification item of the archive specification items" );

		obsoleteSpecificationItem.replacingSpecificationItem = replacingSpecificationItem;

		if( obsoleteSpecificationItem.hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to delete a specification item" );
			}
		else
			{
			if( !isAllowingReplacedObsoleteSpecification )
				{
				if( replaceItem( obsoleteSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace a specification item" );

				if( replacingSpecificationItem != null &&
				( obsoleteJustificationItem = obsoleteSpecificationItem.firstJustificationItem() ) != null &&

				( ( replacingSpecificationItem.isUserSpecification() ||

				( replacingSpecificationItem.isSelfGeneratedQuestion() &&
				!replacingSpecificationItem.isAnsweredQuestion() ) ) &&

				!myWordItem().isJustificationInUse( obsoleteJustificationItem ) ) )
					{
					// Remove justifications from an obsolete specification item
					if( removeJustificationsFromConfirmedAssumptionOrConclusion( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to remove the justifications from a confirmed assumption or conclusion" );
					}
				}
			}

		if( updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the specifications in the justification of involved words" );

		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.hasAnsweredQuestionInJustification() &&
		replaceOrDeleteSpecification( false, replacingSpecificationItem, null ) != Constants.RESULT_OK )
			return addError( 1, "I failed to replace or delete a replacing specification with an answered question in its justification" );

		return Constants.RESULT_OK;
		}

	protected byte updateJustificationInSpecifications( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		boolean isSameJustification;
		JustificationItem attachedPredecessorOfOldJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		WordItem generalizationWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		while( searchSpecificationItem != null )
			{
			// Select self-generated specifications
			if( ( firstJustificationItem = searchSpecificationItem.firstJustificationItem() ) != null )
				{
				// Check first justification
				if( firstJustificationItem == obsoleteJustificationItem )
					{
					// Allowed to change the current justification, without creating a new one
					if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
						{
						if( searchSpecificationItem.changeFirstJustification( replacingJustificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to change the first justification item of a specification item" );

						searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
						}
					else
						{
						createdSpecificationItem = null;

						if( replacingJustificationItem != null )
							{
							if( ( createAndAssignResult = createSpecificationItem( searchSpecificationItem.isInactiveAssignment(), isArchivedAssignment, searchSpecificationItem.isAnsweredQuestion(), searchSpecificationItem.isConcludedAssumption(), searchSpecificationItem.isConditional(), searchSpecificationItem.isCorrectedAssumption(), searchSpecificationItem.isEveryGeneralization(), ( isExclusiveGeneralization || searchSpecificationItem.isExclusiveGeneralization() ), ( isExclusiveGeneralization || searchSpecificationItem.isExclusiveSpecification() ), searchSpecificationItem.isNegative(), searchSpecificationItem.isPartOf(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.isValueSpecification(), searchSpecificationItem.assignmentLevel(), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.languageNr(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.generalizationWordTypeNr(), searchSpecificationItem.specificationWordTypeNr(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationCollectionNr(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), ( isExclusiveGeneralization ? Constants.NO_CONTEXT_NR : searchSpecificationItem.relationContextNr() ), searchSpecificationItem.originalSentenceNr(), searchSpecificationItem.activeSentenceNr(), searchSpecificationItem.inactiveSentenceNr(), searchSpecificationItem.archivedSentenceNr(), searchSpecificationItem.nContextRelations(), replacingJustificationItem, searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), searchSpecificationItem.lastWrittenSentenceStringBuffer ) ).result != Constants.RESULT_OK )
								return addError( 1, "I failed to copy the search specification item" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
								return startError( 1, "I couldn't copy the search specification item" );
							}

						if( replaceOrDeleteSpecification( false, searchSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to replace or delete a specification" );

						if( isExclusiveGeneralization &&
						generalizationWordItem.assignSpecification( false, false, true, false, searchSpecificationItem.isNegative(), searchSpecificationItem.isPartOf(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.originalSentenceNr(), searchSpecificationItem.activeSentenceNr(), searchSpecificationItem.inactiveSentenceNr(), searchSpecificationItem.archivedSentenceNr(), searchSpecificationItem.nContextRelations(), replacingJustificationItem, searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), null ).result != Constants.RESULT_OK )
							return addError( 1, "I failed to create an assignment" );

						searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
						}
					}
				else
					{
					// Check attached justification items
					if( ( attachedPredecessorOfOldJustificationItem = firstJustificationItem.attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) ) == null )
						searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
					else
						{
						if( attachedPredecessorOfOldJustificationItem.hasCurrentCreationSentenceNr() )
							{
							isSameJustification = ( attachedPredecessorOfOldJustificationItem == replacingJustificationItem );

							if( attachedPredecessorOfOldJustificationItem.changeAttachedJustification( isSameJustification ? null : replacingJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, "I failed to change the attached justification item of a justification item" );

							searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
							}
						else
							{
							if( ( justificationResult = generalizationWordItem.copyJustification( false, attachedPredecessorOfOldJustificationItem.primarySpecificationItem(), attachedPredecessorOfOldJustificationItem.secondarySpecificationItem(), replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result != Constants.RESULT_OK )
								return addError( 1, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
								{
								if( generalizationWordItem.replaceJustification( attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchSpecificationItem ) != Constants.RESULT_OK )
									return addError( 1, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );

								searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							else
								searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
							}
						}
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isSpecificationWordSpanishAmbiguous;
		boolean hasRelationWord = ( relationWordItem != null );
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&

				( relationWordItem == null ||
				relationWordItem.hasContextInWord( searchSpecificationItem.relationContextNr(), specificationWordItem ) ) )
					{
					if( ( isPossessive &&
					!isSpecificationWordSpanishAmbiguous ) ||

					( !hasRelationWord &&
					isSpecificationWordSpanishAmbiguous ) )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem.nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.isMatchingRelationContextNr( true, relationContextNr ) )
					{
					nCurrentRelationContextWords = searchSpecificationItem.nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
					{
					if( !hasRelationContext ||
					!searchSpecificationItem.hasRelationContext() ||
					searchSpecificationItem.relationContextNr() == relationContextNr )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem.nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isAllowingEmptyContextResult, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// The given specification word item can be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				!searchSpecificationItem.isSpecificationGeneralization() &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					return searchSpecificationItem;

				if( foundSpecificationItem == null &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					{
					if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
						return searchSpecificationItem;

					// Wait for matching specification collection 
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isQuestion )
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.specificationItem( isIncludingAnsweredQuestions, true, isQuestion );

		return null;
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, short questionParameter )
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.specificationItem( isIncludingAnsweredQuestions, true, questionParameter );

		return null;
		}

	protected SpecificationItem firstExclusiveNounSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.exclusiveNounSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstExclusiveSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.exclusiveSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem specificationWordItem;

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isPossessive() &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null )
				{
				if( specificationWordItem.isFemaleOrMale() )
					return searchSpecificationItem;
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonCompoundUserSpecificationItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasSpecificationNonCompoundCollection() &&
			searchSpecificationItem.isUserSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentOrSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem specificationWordItem;

		if( compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.hasSpecificationCollection() &&
				!searchSpecificationItem.isNegative() &&
				!searchSpecificationItem.isPossessive() &&
				searchSpecificationItem.specificationCollectionNr() != compoundSpecificationCollectionNr &&
				( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null )
					{
					if( searchSpecificationItem.hasSpecificationNonCompoundCollection() &&
					specificationWordItem.hasCollectionNr( compoundSpecificationCollectionNr ) )
						return searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstNonPossessiveDefinitionSpecificationItem( boolean isIncludingAdjectives )
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.nonPossessiveDefinitionSpecificationItem( isIncludingAdjectives, true );

		return null;
		}

	protected SpecificationItem firstPossessiveSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.possessiveSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( boolean isArchivedAssignment, short questionParameter )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( true, false, isArchivedAssignment, questionParameter );

		if( searchSpecificationItem != null )
			return searchSpecificationItem.recentlyAnsweredQuestionSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstRelationSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.relationSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstRelationSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( relationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isPossessive() == isPossessive &&
				relationWordItem.hasContextInWord( searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem() ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption &&
				searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&

				( ( relationWordItem == null &&

				( isAllowingEmptyContextResult ||
				searchSpecificationItem.hasRelationContext() ) ) ||

				( relationWordItem != null &&
				relationWordItem.hasContextInWord( searchSpecificationItem.relationContextNr(), specificationWordItem ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) &&

				( searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr ||

				// Specification collection doesn't match
				// Check for compound collection to avoid duplicates
				( ( relationContextNr == Constants.NO_CONTEXT_NR ||
				!searchSpecificationItem.hasRelationContext() ) &&

				specificationWordItem.isCompoundCollection( specificationCollectionNr ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordTypeNr() == specificationWordTypeNr &&
				searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					{
					if( searchSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification )
						return searchSpecificationItem;

					if( foundSpecificationItem == null )
						foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		WordItem anyWordItem = myWordItem();
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( !searchSpecificationItem.isSpecificationGeneralization() &&
				searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.generalizationContextNr() == generalizationContextNr &&
				searchSpecificationItem.specificationContextNr() == specificationContextNr &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&

				( searchSpecificationItem.relationContextNr() == relationContextNr ||
				anyWordItem.isContextSubsetInContextWords( relationContextNr, searchSpecificationItem.relationContextNr() ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isSpecificationGeneralization() == isSpecificationGeneralization &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstUnhiddenSpecificationItem( boolean isArchivedAssignment, int relationContextNr )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( false, isArchivedAssignment );

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.relationContextNr() == relationContextNr &&
				searchSpecificationItem.wasHiddenSpanishSpecification() )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isUserSpecification() &&
			searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isMatchingRelationContextNr( false, relationContextNr ) &&

			( searchSpecificationItem.specificationWordItem() == specificationWordItem ||

			( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem sameUserQuestionSpecificationItem( boolean isArchivedAssignment, short questionParameter )
		{
		int nUserSpecificationWords = CommonVariables.nUserSpecificationWords;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isUserQuestion() &&
			searchSpecificationItem.nInvolvedSpecificationWords() == nUserSpecificationWords )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected WordItem feminineOrMasculineCommonWordItem( boolean isFeminineWord )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem commonWordItem;
		WordItem specificationWordItem;
		WordItem generalizationWordItem = myWordItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			!searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isPossessive() &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null )
				{
				if( specificationWordItem.isFemaleOrMale() &&
				( commonWordItem = specificationWordItem.commonWordItem( searchSpecificationItem.specificationCollectionNr() ) ) != null )
					{
					if( commonWordItem == generalizationWordItem )
						{
						if( specificationWordItem.isFemale() == isFeminineWord )
							return specificationWordItem;
						}
					else
						{
						if( ( isFeminineWord &&
						commonWordItem.isFemale() ) ||

						( !isFeminineWord &&
						commonWordItem.isMale() ) )
							return commonWordItem;
						}
					}
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected CreateAndAssignResultType createSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer writtenSentenceStringBuffer )
		{
		boolean isAssignment = isAssignmentList();
		WordItem generalizationWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( generalizationWordTypeNr <= Constants.NO_WORD_TYPE_NR &&
		generalizationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( 1, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR &&
		specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( 1, "The given specification word type number is undefined or out of bounds" );

		if( firstJustificationItem != null &&
		!firstJustificationItem.isActiveItem() )
			return startCreateAndAssignResultError( 1, "The given first justification item isn't active" );

		if( specificationWordItem != null &&
		specificationWordItem.isNounValue() )
			return startCreateAndAssignResultError( 1, "The given specification word item is a value word" );

		if( !isMarkedAsSpecificationWord_ )
			{
			isMarkedAsSpecificationWord_ = true;

			if( isAssignment )
				addToAssignmentWordQuickAccessList();
			else
				{
				if( isPossessive &&
				generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR )
					addToPossessiveNounWordQuickAccessList();

				addToSpecificationWordQuickAccessList();
				}
			}

		if( ( createAndAssignResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, generalizationWordItem.isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : Constants.NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, this, generalizationWordItem ) ) == null )
			return startCreateAndAssignResultError( 1, "I failed to create a specification item" );

		if( addItemToList( ( isArchivedAssignment ? Constants.QUERY_ARCHIVED_CHAR : ( isInactiveAssignment ? Constants.QUERY_INACTIVE_CHAR : Constants.QUERY_ACTIVE_CHAR ) ), createAndAssignResult.createdSpecificationItem ) != Constants.RESULT_OK )
			return addCreateAndAssignResultError( 1, "I failed to add an assignment item" );

		if( isAssignment &&
		assignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
		originalSentenceNr == CommonVariables.currentSentenceNr )
			CommonVariables.isAssignmentChanged = true;

		if( writtenSentenceStringBuffer != null )
			createAndAssignResult.createdSpecificationItem.lastWrittenSentenceStringBuffer = new StringBuffer( writtenSentenceStringBuffer );

		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"Good comes to those who lend money generously
 *	and conduct their business fairly.
 *	Such people will not be overcome by evil.
 *	Those who are righteous will be long remembered" (Psalm 112:5-6)
 *************************************************************************/
