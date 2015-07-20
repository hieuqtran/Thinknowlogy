/*
 *	Class:			AdminQuery
 *	Supports class:	AdminItem
 *	Purpose:		To process queries
 *	Version:		Thinknowlogy 2015r1beta (Coraz�n)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class AdminQuery
	{
	// Private constructible variables

	private int queryItemNr_;
	private int querySentenceNr_;
	private int queryStringPosition_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private void clearQuerySelections()
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.clearQuerySelectionsInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				adminItem_.adminListArray[adminListNr].clearQuerySelectionsInList();
			}
		}

	private static boolean isNeedingToShowTotal()
		{
		int nFoundCategories = 0;

		if( CommonVariables.nActiveQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nInactiveQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nArchivedQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nReplacedQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nDeletedQueryItems > 0 )
			nFoundCategories++;

		return ( nFoundCategories > 1 );
		}

	private static boolean isAdminListChar( char queryListChar )
		{
		return ( queryListChar == Constants.ADMIN_FILE_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_READ_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_SCORE_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_WORD_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_CONDITION_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_ACTION_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL );
		}

	private static boolean isWordListChar( char queryListChar )
		{
		return ( queryListChar == Constants.WORD_ASSIGNMENT_LIST_SYMBOL ||
				queryListChar == Constants.WORD_COLLECTION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_GENERALIZATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_INTERFACE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_JUSTIFICATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL ||
				queryListChar == Constants.WORD_GRAMMAR_LIST_SYMBOL ||
				queryListChar == Constants.WORD_WRITE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_SPECIFICATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_TYPE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_CONTEXT_LIST_SYMBOL );
		}

	private static int nTotalCount()
		{
		return ( CommonVariables.nActiveQueryItems +
				CommonVariables.nInactiveQueryItems +
				CommonVariables.nArchivedQueryItems +
				CommonVariables.nReplacedQueryItems +
				CommonVariables.nDeletedQueryItems );
		}

	private int countQuery()
		{
		WordList wordList;

		CommonVariables.nActiveQueryItems = 0;
		CommonVariables.nInactiveQueryItems = 0;
		CommonVariables.nArchivedQueryItems = 0;
		CommonVariables.nReplacedQueryItems = 0;
		CommonVariables.nDeletedQueryItems = 0;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.countQueryInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				adminItem_.adminListArray[adminListNr].countQueryInList();
			}

		return nTotalCount();
		}

	private byte itemQuery( boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isSuppressingMessage, StringBuffer textStringBuffer )
		{
		if( textStringBuffer != null )
			{
			if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) == Constants.RESULT_OK )
				{
				if( CommonVariables.currentLanguageWordItem != null )
					{
					if( !isSuppressingMessage &&
					countQuery() == 0 )
						textStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The current language word item is undefined" );
				}
			else
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query items" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given text string buffer is undefined" );

		return Constants.RESULT_OK;
		}

	private byte showQueryResult( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, ( queryWordTypeNr == Constants.WORD_TYPE_UNDEFINED ? CommonVariables.matchingWordTypeNr : queryWordTypeNr ), queryWidth ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to show the query result in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].showQueryResultInList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to show the query result" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte getIdFromQuery( boolean hasEndChar, String sourceString, char startChar, char endChar )
		{
		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryItemNr_ = Constants.NO_ITEM_NR;

		if( sourceString != null )
			{
			if( queryStringPosition_ >= 0 )
				{
				if( queryStringPosition_ < sourceString.length() )
					{
					if( sourceString.charAt( queryStringPosition_ ) == startChar )
						{
						queryStringPosition_++;

						if( queryStringPosition_ < sourceString.length() &&
						sourceString.charAt( queryStringPosition_ ) != endChar )
							{
							if( sourceString.charAt( queryStringPosition_ ) == Constants.SYMBOL_ASTERISK )
								{
								queryStringPosition_++;
								querySentenceNr_ = Constants.MAX_SENTENCE_NR;
								}
							else
								{
								if( sourceString.charAt( queryStringPosition_ ) != Constants.QUERY_SEPARATOR_CHAR )
									{
									if( Character.isDigit( sourceString.charAt( queryStringPosition_ ) ) )
										{
										while( queryStringPosition_ < sourceString.length() &&
										Character.isDigit( sourceString.charAt( queryStringPosition_ ) ) &&
										querySentenceNr_ <= Constants.MAX_SENTENCE_NR / 10 )
											{
											querySentenceNr_ = ( querySentenceNr_ * 10 ) + ( sourceString.charAt( queryStringPosition_ ) - '0' );
											queryStringPosition_++;
											}
										}
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find a number in the query string" );
									}

								if( hasEndChar &&
								queryStringPosition_ < sourceString.length() &&
								sourceString.charAt( queryStringPosition_ ) == Constants.QUERY_SEPARATOR_CHAR )
									{
									queryStringPosition_++;

									if( queryStringPosition_ < sourceString.length() &&
									sourceString.charAt( queryStringPosition_ ) != endChar )
										{
										if( Character.isDigit( sourceString.charAt( queryStringPosition_ ) ) )
											{
											while( queryStringPosition_ < sourceString.length() &&
											Character.isDigit( sourceString.charAt( queryStringPosition_ ) ) &&
											queryItemNr_ <= Constants.MAX_SENTENCE_NR / 10 )
												{
												queryItemNr_ = ( queryItemNr_ * 10 ) + ( sourceString.charAt( queryStringPosition_ ) - '0' );
												queryStringPosition_++;
												}
											}
										else
											return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the item number in the query string" );
										}
									}
								}
							}

						if( hasEndChar )
							{
							if( queryStringPosition_ < sourceString.length() &&
							sourceString.charAt( queryStringPosition_ ) == endChar )
								queryStringPosition_++;
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "The given query string is corrupt" );
							}
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given command doesn't start with character '" + startChar + "', but with '" + sourceString.charAt( queryStringPosition_ ) + "'" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given source string is empty or the given query source string position is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given query source string position is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given source string is undefined" );

		return Constants.RESULT_OK;
		}

	private byte getNameFromQuery( String sourceString, StringBuffer nameStringBuffer, char startChar, char endChar )
		{
		int startSourceStringPosition;

		if( sourceString != null )
			{
			if( nameStringBuffer != null )
				{
				if( queryStringPosition_ >= 0 )
					{
					if( queryStringPosition_ < sourceString.length() )
						{
						if( sourceString.charAt( queryStringPosition_ ) == startChar )
							{
							if( queryStringPosition_ + 1 < sourceString.length() )
								{
								queryStringPosition_++;

								if( sourceString.charAt( queryStringPosition_ ) != endChar )
									{
									startSourceStringPosition = queryStringPosition_;

									while( queryStringPosition_ + 1 < sourceString.length() &&
									sourceString.charAt( queryStringPosition_ ) != endChar )
										queryStringPosition_++;

									if( sourceString.charAt( queryStringPosition_ ) == endChar )
										nameStringBuffer.append( sourceString.substring( startSourceStringPosition, queryStringPosition_ ) );
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "The name in the given query string is corrupt" );
									}

								queryStringPosition_++;
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "The name in the given query string is corrupt" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The given name doesn't start with character '" + startChar + "', but with '" + sourceString.charAt( 0 ) + "'" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given source string is empty or the given query source string position is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given query source string position is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given name string is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given source string is undefined" );

		return Constants.RESULT_OK;
		}

	private byte itemQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.itemQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query item numbers in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].itemQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to query item numbers in" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte listQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, StringBuffer queryListStringBuffer )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.listQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].listQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to do a list query module" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte wordTypeQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordTypeQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query word types in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].wordTypeQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to word types" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte parameterQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.parameterQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query parameters in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].parameterQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to parameters" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte wordQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordNameString )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query the words in my word list" );
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordReferenceQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query word references in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].wordReferenceQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte stringQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String queryString )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.stringQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query strings in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].stringQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to query strings" );
				}
			}

		return Constants.RESULT_OK;
		}

	// Constructor / deconstructor

	protected AdminQuery( AdminItem adminItem )
		{
		String errorString = null;

		queryItemNr_ = Constants.NO_ITEM_NR;
		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryStringPosition_ = 1;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeQueryStringPosition()
		{
		queryStringPosition_ = 1;
		}

	protected byte writeTextWithPossibleQueryCommands( short promptTypeNr, String textString )
		{
		boolean hasFoundNewLine = false;
		int previousPosition;
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer writeStringBuffer = new StringBuffer();

		if( textString != null )
			{
			if( textString.charAt( 0 ) == Constants.SYMBOL_DOUBLE_QUOTE )
				position++;

			while( position < textString.length() &&
			textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE )
				{
				if( textString.charAt( position ) == Constants.QUERY_CHAR )
					{
					if( ++position < textString.length() )
						{
						previousPosition = position;
						queryStringPosition_ = position;

						if( executeQuery( true, false, true, promptTypeNr, textString ) == Constants.RESULT_OK )
							position = queryStringPosition_;
						else
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute query \"" + textString.substring( previousPosition ) + "\"" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The text string ended with a query character" );
					}
				else
					{
					if( textString.charAt( position ) == Constants.TEXT_DIACRITICAL_CHAR )
						{
						if( ++position < textString.length() )
							{
							if( ( textChar = Presentation.convertDiacriticalChar( textString.charAt( position ) ) ) == Constants.NEW_LINE_CHAR )
								hasFoundNewLine = true;
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The text string ended with a diacritical sign" );
						}
					else
						textChar = textString.charAt( position );

					position++;
					writeStringBuffer.append( textChar );
					}

				if( hasFoundNewLine ||

				( position < textString.length() &&
				textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE &&
				textString.charAt( position ) == Constants.QUERY_CHAR &&
				writeStringBuffer.length() > 0 ) )
					{
					if( Presentation.writeText( false, promptTypeNr, Constants.NO_CENTER_WIDTH, writeStringBuffer ) == Constants.RESULT_OK )
						{
						hasFoundNewLine = false;
						writeStringBuffer = new StringBuffer( Constants.EMPTY_STRING );
						}
					else
						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a character" );
					}
				}

			if( writeStringBuffer.length() > 0 )
				{
				if( Presentation.writeText( false, promptTypeNr, Constants.NO_CENTER_WIDTH, writeStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the last characters" );
				}
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given text string is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, String queryString )
		{
		boolean isEndOfQuery = false;
		boolean isFirstInstruction = true;
		boolean isInvalidChar = false;
		boolean isOnlyShowingStrings = false;
		boolean isOnlyShowingWords = false;
		boolean isOnlyShowingWordReferences = false;
		boolean isReturnQueryToPosition = isReturningToPosition;
		boolean isSelectActiveItems = true;
		boolean isSelectInactiveItems = true;
		boolean isSelectArchivedItems = true;
		boolean isSelectReplacedItems = true;
		boolean isSelectDeletedItems = true;
		boolean isShowingCount = false;
		short queryWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		int nTotal;
		int listStringPosition;
		int queryWidth = Constants.NO_CENTER_WIDTH;
		StringBuffer nameStringBuffer = new StringBuffer();

		CommonVariables.queryStringBuffer = new StringBuffer();

		if( queryString != null )
			{
			if( queryStringPosition_ > 0 &&
			queryStringPosition_ < queryString.length() )
				{
				if( CommonVariables.currentLanguageWordItem != null )
					{
					clearQuerySelections();

					querySentenceNr_ = Constants.NO_SENTENCE_NR;
					queryItemNr_ = Constants.NO_ITEM_NR;

					CommonVariables.hasFoundQuery = false;
					CommonVariables.matchingWordTypeNr = Constants.WORD_TYPE_UNDEFINED;

					while( !isEndOfQuery &&
					CommonVariables.queryStringBuffer.length() == 0 &&
					queryStringPosition_ < queryString.length() )
						{
						switch( queryString.charAt( queryStringPosition_ ) )
							{
							case Constants.QUERY_ITEM_START_CHAR:

								if( getIdFromQuery( true, queryString, Constants.QUERY_ITEM_START_CHAR, Constants.QUERY_ITEM_END_CHAR ) == Constants.RESULT_OK )
									{
									if( itemQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, querySentenceNr_, queryItemNr_ ) == Constants.RESULT_OK )
										{
										isFirstInstruction = false;

										if( !isSuppressingMessage &&
										countQuery() == 0 )
											CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEM_WAS_FOUND ) );
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query items" );
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get an identification from the item" );

								break;

							case Constants.QUERY_REF_ITEM_START_CHAR:

								if( getIdFromQuery( true, queryString, Constants.QUERY_REF_ITEM_START_CHAR, Constants.QUERY_REF_ITEM_END_CHAR ) == Constants.RESULT_OK )
									{
									if( itemQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, true, querySentenceNr_, queryItemNr_ ) == Constants.RESULT_OK )
										{
										isFirstInstruction = false;

										if( !isSuppressingMessage &&
										countQuery() == 0 )
											CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND ) );
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query item references" );
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a reference identification from the item" );

								break;

							case Constants.QUERY_LIST_START_CHAR:
								nameStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

								if( getNameFromQuery( queryString, nameStringBuffer, Constants.QUERY_LIST_START_CHAR, Constants.QUERY_LIST_END_CHAR ) == Constants.RESULT_OK )
									{
									listStringPosition = 0;

									// Check list characters for existence
									do	{
										if( nameStringBuffer.length() > 0 &&
										!isWordListChar( nameStringBuffer.charAt( listStringPosition ) ) &&
										!isAdminListChar( nameStringBuffer.charAt( listStringPosition ) ) )
											{
											isInvalidChar = true;
											CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST ) );
											}
										}
									while( !isInvalidChar &&
									++listStringPosition < nameStringBuffer.length() );

									// All list characters are valid
									if( !isInvalidChar )
										{
										if( listQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameStringBuffer ) == Constants.RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_LIST_WAS_FOUND ) );
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query lists" );
										}
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a list string from the text" );

								break;

							case Constants.QUERY_WORD_START_CHAR:
								if( queryStringPosition_ + 1 < queryString.length() &&
								queryString.charAt( queryStringPosition_ + 1 ) != Constants.QUERY_CHAR )
									{
									nameStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

									if( getNameFromQuery( queryString, nameStringBuffer, Constants.QUERY_WORD_START_CHAR, Constants.QUERY_WORD_END_CHAR ) == Constants.RESULT_OK )
										{
										if( nameStringBuffer.length() == 0 )
											{
											if( queryStringPosition_ < queryString.length() &&
											queryString.charAt( queryStringPosition_ ) != Constants.QUERY_CHAR )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION ) );
											else
												{
												isOnlyShowingWords = true;
												isReturnQueryToPosition = true;
												}
											}
										else
											{
											if( wordQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameStringBuffer.toString() ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_WAS_FOUND ) );
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query words" );
											}
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a word name from the query specification" );
									}
								else
									{
									isOnlyShowingWords = true;
									isReturnQueryToPosition = false;
									queryStringPosition_++;
									}

								break;

							case Constants.QUERY_WORD_REFERENCE_START_CHAR:
								if( queryStringPosition_ + 1 < queryString.length() &&
								queryString.charAt( queryStringPosition_ + 1 ) != Constants.QUERY_CHAR )
									{
									nameStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

									if( getNameFromQuery( queryString, nameStringBuffer, Constants.QUERY_WORD_REFERENCE_START_CHAR, Constants.QUERY_WORD_REFERENCE_END_CHAR ) == Constants.RESULT_OK )
										{
										if( nameStringBuffer.length() == 0 )
											{
											if( queryStringPosition_ < queryString.length() &&
											queryString.charAt( queryStringPosition_ ) != Constants.QUERY_CHAR )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_REFERENCE ) );
											else
												{
												isReturnQueryToPosition = true;
												isOnlyShowingWordReferences = true;
												}
											}
										else
											{
											if( wordReferenceQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameStringBuffer.toString() ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND ) );
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query word references" );
											}
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a word reference name from the query specification" );
									}
								else
									{
									isReturnQueryToPosition = false;
									isOnlyShowingWordReferences = true;
									queryStringPosition_++;
									}

								break;

							case Constants.SYMBOL_BACK_SLASH:		// Escape character for string
								if( queryStringPosition_ + 1 < queryString.length() &&
								queryString.charAt( queryStringPosition_ + 1 ) != Constants.QUERY_CHAR )
									queryStringPosition_++;

								// Don't insert a break statement here

							case Constants.QUERY_STRING_START_CHAR:
								if( queryStringPosition_ + 1 < queryString.length() &&
								queryString.charAt( queryStringPosition_ + 1 ) != Constants.QUERY_CHAR )
									{
									nameStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

									if( getNameFromQuery( queryString, nameStringBuffer, Constants.QUERY_STRING_START_CHAR, Constants.QUERY_STRING_END_CHAR ) == Constants.RESULT_OK )
										{
										if( nameStringBuffer.length() == 0 )
											{
											if( queryStringPosition_ < queryString.length() &&
											queryString.charAt( queryStringPosition_ ) != Constants.QUERY_CHAR )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION ) );
											else
												{
												isOnlyShowingStrings = true;
												isReturnQueryToPosition = true;
												}
											}
										else
											{
											if( stringQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameStringBuffer.toString() ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_STRING_WAS_FOUND ) );
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query strings" );
											}
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a string from the query specification" );
									}
								else
									{
									isOnlyShowingStrings = true;
									isReturnQueryToPosition = false;
									queryStringPosition_++;
									}

								break;

							case Constants.QUERY_WORD_TYPE_CHAR:

								querySentenceNr_ = Constants.NO_SENTENCE_NR;

								if( getIdFromQuery( false, queryString, Constants.QUERY_WORD_TYPE_CHAR, Constants.QUERY_WORD_TYPE_CHAR ) == Constants.RESULT_OK )
									{
									if( queryItemNr_ == Constants.NO_ITEM_NR )
										{
										if( wordTypeQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, (short)querySentenceNr_ ) == Constants.RESULT_OK )
											{
											isFirstInstruction = false;
											// Remember given word type number
											queryWordTypeNr = (short)querySentenceNr_;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND ) );
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query word types" );
										}
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "The given parameter is undefined" );
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a word type" );

								break;

							case Constants.QUERY_PARAMETER_CHAR:

								querySentenceNr_ = Constants.NO_SENTENCE_NR;

								if( getIdFromQuery( false, queryString, Constants.QUERY_PARAMETER_CHAR, Constants.QUERY_PARAMETER_CHAR ) == Constants.RESULT_OK )
									{
									if( queryItemNr_ == Constants.NO_ITEM_NR )
										{
										if( parameterQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, querySentenceNr_ ) == Constants.RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND ) );
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query parameters" );
										}
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "The given parameter is undefined" );
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a parameter" );

								break;

							case Constants.QUERY_CHAR:

								isEndOfQuery = true;
								queryStringPosition_++;

								break;

							case Constants.QUERY_ACTIVE_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectActiveItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to perform an item query of active items" );
									}

								break;

							case Constants.QUERY_INACTIVE_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectInactiveItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to perform an item query of inactive items" );
									}

								break;

							case Constants.QUERY_ARCHIVED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectArchivedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to perform an item query of archived items" );
									}

								break;

							case Constants.QUERY_REPLACED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectReplacedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to perform an item query of archived items" );
									}

								break;

							case Constants.QUERY_DELETED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									}
								else
									isSelectDeletedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to perform an item query of deleted items" );
									}

								break;

							case Constants.QUERY_COUNT_CHAR:

								isShowingCount = true;
								queryStringPosition_++;

								if( queryStringPosition_ >= queryString.length() ||
								// End of query
								queryString.charAt( queryStringPosition_ ) == Constants.QUERY_CHAR )
									{
									if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query items" );
									}

								break;

							default:
								// Set query width parameter
								if( Character.isDigit( queryString.charAt( queryStringPosition_ ) ) )
									{
									while( queryStringPosition_ < queryString.length() &&
									Character.isDigit( queryString.charAt( queryStringPosition_ ) ) &&
									queryWidth <= Constants.MAX_SENTENCE_NR / 10 )
										{
										queryWidth = ( queryWidth * 10 + queryString.charAt( queryStringPosition_ ) - '0' );
										queryStringPosition_++;
										}
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an illegal character '" + queryString.charAt( queryStringPosition_ ) + "' in the query" );
							}
						}

					if( CommonVariables.queryStringBuffer.length() == 0 )
						{
						// No query performed yet
						if( isFirstInstruction )
							{
							if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to query items" );
							}

						if( isShowingCount )
							{
							nTotal = countQuery();

							if( isSuppressingMessage )
								CommonVariables.queryStringBuffer.append( nTotal );
							else
								{
								if( nTotal == 0 )
									CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
								else
									{
									if( isNeedingToShowTotal() )
										CommonVariables.queryStringBuffer.append( "total:" + nTotal );

									if( CommonVariables.nActiveQueryItems > 0 )
										{
										if( CommonVariables.queryStringBuffer.length() > 0 )
											CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

										CommonVariables.queryStringBuffer.append( "active:" + CommonVariables.nActiveQueryItems );
										}

									if( CommonVariables.nInactiveQueryItems > 0 )
										{
										if( CommonVariables.queryStringBuffer.length() > 0 )
											CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

										CommonVariables.queryStringBuffer.append( "inactive:" + CommonVariables.nInactiveQueryItems );
										}

									if( CommonVariables.nArchivedQueryItems > 0 )
										{
										if( CommonVariables.queryStringBuffer.length() > 0 )
											CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

										CommonVariables.queryStringBuffer.append( "archived:" + CommonVariables.nArchivedQueryItems );
										}

									if( CommonVariables.nReplacedQueryItems > 0 )
										{
										if( CommonVariables.queryStringBuffer.length() > 0 )
											CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

										CommonVariables.queryStringBuffer.append( "replaced:" + CommonVariables.nReplacedQueryItems );
										}

									if( CommonVariables.nDeletedQueryItems > 0 )
										{
										if( CommonVariables.queryStringBuffer.length() > 0 )
											CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

										CommonVariables.queryStringBuffer.append( "deleted:" + CommonVariables.nDeletedQueryItems );
										}
									}
								}
							}
						else
							{
							CommonVariables.hasFoundQuery = false;

							if( showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == Constants.RESULT_OK )
								{
								if( !isSuppressingMessage &&
								!CommonVariables.hasFoundQuery &&
								CommonVariables.queryStringBuffer.length() == 0 )
									{
									if( isOnlyShowingWords )
										CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORDS_WERE_FOUND ) );
									else
										{
										if( isOnlyShowingWordReferences )
											CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND ) );
										else
											{
											if( isOnlyShowingStrings )
												CommonVariables.queryStringBuffer.append( CommonVariables.currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_STRINGS_WERE_FOUND ) );
											}
										}
									}
								}
							else
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to show the query result" );
							}
						}

					if( nTotalCount() == 0 ||	// Show comment on empty query
					CommonVariables.hasFoundQuery ||
					queryWidth > Constants.NO_CENTER_WIDTH )
						{
						if( isWritingQueryResult &&

						( queryWidth > Constants.NO_CENTER_WIDTH ||
						CommonVariables.queryStringBuffer.length() > 0 ) )
							{
							if( Presentation.writeText( ( !isSuppressingMessage && !CommonVariables.hasFoundQuery && queryWidth == Constants.NO_CENTER_WIDTH ), promptTypeNr, queryWidth, CommonVariables.queryStringBuffer ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the query result" );
							}
						}
					else
						CommonVariables.queryStringBuffer = new StringBuffer();
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The current language word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given instruction string buffer is empty or the given instruction string position is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given instruction string buffer is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Honor the Lord, you heavenly beings;
 *	honor the Lord for his glory and strength." (Psalm 29:1)
 *************************************************************************/