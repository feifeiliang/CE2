#include "./Header/CFG.h"
#include "Header/Parent.h"
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
CFG::CFG() {
}

CFG::~CFG() {
}

static int dummy = -1;

static int currentPro = 0;
static int currentP = 0;
std::vector<int> proRecord;
std::vector<std::vector<std::vector<int> >> CFGTable;
std::vector<std::vector<int>> CFGstmt;
std::vector<int> CFGline;

static int numOfStatement = 0;

std::vector<std::vector<int>> revCFGstmt;
std::vector<int> revCFGline;

std::map <string, int> procedureMap;
map<int, int> startEndOfProcedure;
int conditionstmt;
int prevStmtNo;
stack<int> conditionStack;
stack<int> parentStack;
stack<int> proStack;
stack <int> closingStack;
stack <int> clostingCondition;
stack <int> ifRecord;

stack<int> stackForFirstStmtOfProcedure;
stack<int> stackForFirstStmtOfProcedureCondition;

bool flagNextLevelStart = false;
bool flagForElseStart = false;;
bool flagForClose = false;
bool flag = false;
bool flagForCorrectElseIf = false;
bool flagForNewProc = false;

int lastWhile = 0;
int lastIf = 0;
int lastElse = 0;

int endloop;

vector<int> resultStar;
map<int, int> resultStarMap;
map<int, int>::iterator itStar;


int setConditions(string stmtLine);
string trimString(string stmt);


void CFG::addRoot(string procedure, int stmtNo) {
     flagForNewProc == true;
	/*if (procedure.find("procedure ") != string::npos) {
		if (startEndOfProcedure.size() == 0) {
			startEndOfProcedure.insert(pair<int, int>(stmtNo + 1, -1));
			proStack.push(stmtNo + 1);
		}
		else {
			currentP++;
			startEndOfProcedure.insert(pair<int, int>(stmtNo + 1, -1));
			int index = proStack.top();
			proStack.pop();
			proStack.push(stmtNo + 1);
			startEndOfProcedure.at(index) = stmtNo;
			flagForNewProc = true;
		}
		CFGline.clear();
		if (CFGstmt.size() == 0) {
			CFGstmt.clear();
			CFGTable.push_back(CFGstmt);
		}
	}

	flagNextLevelStart = false;
	flagForElseStart = false;;
	flagForClose = false;
	flag = false;
	flagForCorrectElseIf = false;
	while (conditionStack.size() != 0) {
		conditionStack.pop();
	}*/

}

void CFG::addNextNode(int stmtNo, string stmt) {
	numOfStatement = stmtNo;
	CFGline.clear();
	int parentForElseIf = -1;
	stmt = trimString(stmt);
	conditionstmt = setConditions(stmt);
	endloop = std::count(stmt.begin(), stmt.end(), '}');
	// if there is start of new procedure
	if (flagForNewProc) {
		if (CFGstmt.size() == 0) {
		    CFGline.clear();
			CFGline.push_back(stmtNo);
			CFGstmt.push_back(CFGline);
		}
		else {
			if (conditionstmt != 0) {
				parentStack.push(stmtNo);
				conditionStack.push(conditionstmt);
			}
			stackForFirstStmtOfProcedure.push(stmtNo);
			stackForFirstStmtOfProcedureCondition.push(conditionstmt);
		}
		flagForNewProc = false;
	}
	else {
		if (stackForFirstStmtOfProcedure.size() != 0 && stackForFirstStmtOfProcedure.size()!=0 && endloop == 0) {
			if (stackForFirstStmtOfProcedure.top() == CFGstmt.size() - 1) {
				if (conditionstmt != 0) {
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
				}
				CFGline.clear();
				CFGline.push_back(stmtNo);
				CFGstmt.push_back(CFGline);
			}
		}
		else {
			if (endloop == 0) {
				if (conditionstmt == 1) {
					if (!closingStack.empty() && !clostingCondition.empty()) {
					   // pop all the index form the stack to point to next node. 
						while (!closingStack.empty() && !clostingCondition.empty()) {
							int  index = closingStack.top();
							int  cond = clostingCondition.top();
							if (cond == 3) { // if iprev closing is while loop, let the while loop point to current no
								if (CFGstmt.size() > index) {
									CFGline = CFGstmt.at(index);
									CFGline.push_back(stmtNo);
									CFGstmt.at(index) = CFGline;
									closingStack.pop();
									clostingCondition.pop();
								}
							}
							else if (cond == 2) {// if prev closing is else loop, let the endif and endelse point to current no
								if (CFGstmt.size() > index) {
									CFGline = CFGstmt.at(index);
									CFGline.push_back(stmtNo);
									CFGstmt.at(index) = CFGline;
									closingStack.pop();
									clostingCondition.pop();
								}
								index = closingStack.top();
								cond = clostingCondition.top();
								if (cond == 1) {
									if (CFGstmt.size() > index) {
										CFGline = CFGstmt.at(index);
										CFGline.push_back(stmtNo);
										CFGstmt.at(index) = CFGline;
										closingStack.pop();
										clostingCondition.pop();
									}
								}
								else {
									break;
								}
							}
							else if (cond == 1) {
								break;  // still need to wait for the endelse
							}
						}
					}
				   CFGline.clear();
				   parentStack.push(stmtNo);
				   conditionStack.push(conditionstmt);
				   CFGline.push_back(stmtNo);
				   CFGstmt.push_back(CFGline);
				}
				else if (conditionstmt == 2) { // if there is else {
					int pa;
					int con;
					if (!parentStack.empty() && !conditionStack.empty()) {
					    pa = parentStack.top();
						con = conditionStack.top();
						if (CFGstmt.size() > pa) {
							CFGline = CFGstmt.at(pa);
							CFGline.push_back(stmtNo + 1);
							CFGstmt.at(pa) = CFGline;
							parentStack.push(stmtNo);
							conditionStack.push(conditionstmt);
						}
						
					}
				}
				else if (conditionstmt == 3) {  // if next is while 
					if (!closingStack.empty() && !clostingCondition.empty()) {
						// pop all the index form the stack to point to next node. 
						while (!closingStack.empty() && !clostingCondition.empty()) {
							int  index = closingStack.top();
							int  cond = clostingCondition.top();
							if (cond == 3) { // if iprev closing is while loop, let the while loop point to current no
								if (CFGstmt.size() > index) {
									CFGline = CFGstmt.at(index);
									CFGline.push_back(stmtNo);
									CFGstmt.at(index) = CFGline;
									closingStack.pop();
									clostingCondition.pop();
								}
							}
							else if (cond == 2) {// if prev closing is else loop, let the endif and endelse point to current no
								if (CFGstmt.size() > index) {
									CFGline = CFGstmt.at(index);
									CFGline.push_back(stmtNo);
									CFGstmt.at(index) = CFGline;
									closingStack.pop();
									clostingCondition.pop();
								}
								index = closingStack.top();
								cond = clostingCondition.top();
								if (cond == 1) {
									if (CFGstmt.size() > index) {
										CFGline = CFGstmt.at(index);
										CFGline.push_back(stmtNo);
										CFGstmt.at(index) = CFGline;
										closingStack.pop();
										clostingCondition.pop();
									}
								}
								else {
									break;
								}
							}
							else if (cond == 1) {
								break;  // still need to wait for the endelse
							}
						}
					}
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
					CFGline.push_back(stmtNo);
					CFGstmt.push_back(CFGline);
				}
				else {  // normal stmt, just push_back
					CFGline.push_back(stmtNo);
					CFGstmt.push_back(CFGline);
					CFGTable.at(currentPro) = CFGstmt;
				}
			}
			else if (endloop > 0) {
			    if (conditionstmt == 2){ // } else { 
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
				}
				else {
					while (endloop != 0) {
						if (parentStack.empty() && conditionStack.empty()) { // end of procedure
							while (!conditionStack.empty()) {
							   conditionStack.pop();
							}
							while (!proStack.empty()) {
							   proStack.pop();
							}
							while (!closingStack.empty()) {
								closingStack.pop();
							}
							while (!clostingCondition.empty()) {
								clostingCondition.pop();
							}
							while (!ifRecord.empty()) {
								ifRecord.pop();
							}
							break;
						}
						else {
							if (!parentStack.empty() && !conditionStack.empty()) {  // if parent stack is not empty
								int parent = parentStack.top();
								int con = conditionStack.top();
								if (con == 1) { //do nothing  closing of if } \n else { 
								 // assign dummy node to end if;
									/*if (CFGstmt.size() == stmtNo) {
										CFGline.push_back(dummy);
										CFGstmt.push_back(CFGline);
									}
									else {
										cout << "error 207" << endl;
									}*/
								 
								}
								else if (con == 2) { // closing of all if-else
									if (parentStack.size() >= 2 && conditionStack.size() >= 2) {
										// assign dummy node to end if and end else
										// push the dummy node to the end else
										int p = parentStack.top();
									    closingStack.push(p);
										parentStack.pop();
										if (CFGstmt.size() == stmtNo) {
											CFGline.push_back(dummy);
											CFGstmt.push_back(CFGline);
										}
										else {
											cout << "stmtNo is " << stmtNo << " string is " << stmt <<endl;
										}

										p = parentStack.top();
										closingStack.push(p);
										parentStack.pop();

										if (CFGstmt.size() > p) {
											CFGline = CFGstmt.at(p);
											CFGline.push_back(dummy);
											CFGstmt.at(p) = CFGline;
										}
										// push then into closing stack, so that later will update with next line it meet
										clostingCondition.push(conditionStack.top());
										conditionStack.pop();
										clostingCondition.push(conditionStack.top());
										conditionStack.pop();
										// store the end node for if and else, assign each of this to a dummy node and update later
									
									}
								}
								else if (con == 3) { // closing of while check with the closing stack, if the closing stack have something inside, means, need to point back to the while
								   // return back to its parent
								   if (!closingStack.empty() && !clostingCondition.empty()){
								      while (!closingStack.empty() && !clostingCondition.empty()){
									      int indexs = closingStack.top();
										  int condi = clostingCondition.top();
										  if (condi == 3) { // prev closing is while , point the prev closing while to current end while
											  if (CFGstmt.size() > indexs) {
												  CFGline = CFGstmt.at(indexs);
												  CFGline.push_back(parent);
												  CFGstmt.at(indexs) = CFGline;
												  closingStack.pop();
												  clostingCondition.pop();
											  }
											  else {
											      cout << "error in error index" << endl;
												  break;
											  }
										  }
										  else if (condi == 2) { // prev closing is endelse, point the endelse and endif back to while
											  if (CFGstmt.size() > indexs) {
												  CFGline= CFGstmt.at(indexs);
												  for (int i = 0; i < CFGline.size(); i++) {
													  if (CFGline.at(i) == -1) { // update the dummy node
													    CFGline.at(i) == parent;
														break;
													  }
												  }
												  closingStack.pop();
												  clostingCondition.pop();
											  }
											  indexs = closingStack.top();
											  condi = clostingCondition.top();
											  if (condi == 1) {
												  if (CFGstmt.size() > indexs) {
													  CFGline = CFGstmt.at(indexs);
													  for (int i = 0; i < CFGline.size(); i++) {
														  if (CFGline.at(i) == -1) { // update the dummy node
															  CFGline.at(i) == parent;
															  break;
														  }
													  }
													  closingStack.pop();
													  clostingCondition.pop();
												  }
											  }
											  else {
											      cout<<"error in 377, error in indexes"<<endl;
												  break;
											  }

										  }
										  else if (condi == 1) {
										        cout << "wait for end else, don do anything"<<endl;
												break;
										  }
									  }
								   }
								   else { // closing stack is empty; which means it is new end of while, nothing before it, no need to point back  to prev closing
									   if (CFGstmt.size() == stmtNo + 1) { // it is new line, not inside the table
										   CFGline.push_back(parent);
										   CFGstmt.push_back(CFGline);
									   }
									   else {
										   CFGline = CFGstmt.at(stmtNo);
										   CFGline.push_back(parent);
										   CFGstmt.at(stmtNo) = CFGline;
									   }
								   }
								   
								   parentStack.pop();
								   conditionStack.pop();
								   // assign a dummy node to parent and wait for next stmt to update
								   CFGline = CFGstmt.at(parent);
								   CFGline.push_back(dummy);
								   CFGstmt.at(parent) = CFGline;
								   // push the parent to closing stack which means later it will need to point to next stmt
								   closingStack.push(parent);
								   clostingCondition.push(con);
								}
							}
						}
					}
				}
			}
		}
		
	}
	CFGTable.at(currentPro) = CFGstmt;
	
	
	/*
	try {
		numOfStatement = stmtNo;
		CFGline.clear();
		int parentForElseIf = -1;
		stmt = trimString(stmt);
		conditionstmt = setConditions(stmt);
		CFGline.clear();
		if (stmt.compare("}") != 0 && stmt.find("else ") == string::npos) {
			proRecord.push_back(currentP);
		}
		endloop = std::count(stmt.begin(), stmt.end(), '}');
		if (flagForClose && stmt.size() != endloop) {
			if (conditionstmt == 3) {
				if (parentStack.size() != 0) {
					parentForElseIf = parentStack.top();
				}
				parentStack.push(stmtNo);
				conditionStack.push(conditionstmt);
				if (closingStack.size() != 0) {
					int prev = closingStack.top();
					closingStack.pop();
					int prevCon = clostingCondition.top();
					clostingCondition.pop();
					if (prevCon == 3) {
						CFGline = CFGTable.at(currentPro).at(prev);
						CFGline.push_back(stmtNo);
						CFGstmt.at(prev) = CFGline;
						CFGTable.at(currentPro) = CFGstmt;
					}
					else {
						if (prevCon == 2) {
							CFGline.push_back(dummy);
							CFGstmt.push_back(CFGline);
							CFGTable.at(currentPro) = CFGstmt;
						}
						else {
							if (parentForElseIf != -1) {
								CFGline = CFGTable.at(currentPro).at(parentForElseIf);
								CFGline.push_back(stmtNo);
								CFGstmt.at(parentForElseIf) = CFGline;
								CFGTable.at(currentPro) = CFGstmt;
							}
							else {
								CFGline.push_back(dummy);
								CFGstmt.push_back(CFGline);
								CFGTable.at(currentPro) = CFGstmt;
							}

						}
					}
				}
			}
			else if (conditionstmt == 2) {
				if (endloop > 0) {
					int parent = parentStack.top();
					parentStack.push(parent);
					conditionStack.push(conditionstmt);
					int prev = closingStack.top();
					closingStack.pop();
					int con = clostingCondition.top();
					clostingCondition.pop();
					if (con == 3) {
						CFGline = CFGTable.at(currentPro).at(prev);
						CFGline.push_back(stmtNo);
						CFGstmt.at(prev) = CFGline;
						CFGTable.at(currentPro) = CFGstmt;
					}
					else {
					}
				}
				else {
					int parent = 0;
					if (ifRecord.size() > 0) {
						parent = ifRecord.top();
						ifRecord.pop();
						parentStack.push(parent);
						conditionStack.push(1);
					}


					flagForCorrectElseIf = true;
					flag = true;

				}
			}
			else if (conditionstmt == 1) {
				if (parentStack.size() != 0) {
					parentForElseIf = parentStack.top();
				}
				else {
					parentForElseIf = -1;
				}
				parentStack.push(stmtNo);
				conditionStack.push(conditionstmt);
				if (closingStack.size() != 0) {
					int prev = closingStack.top();
					closingStack.pop();
					int prevCon = clostingCondition.top();
					clostingCondition.pop();
					if (prevCon == 3) {
						CFGline = CFGTable.at(currentPro).at(prev);
						CFGline.push_back(stmtNo);
						CFGstmt.at(prev) = CFGline;
						CFGTable.at(currentPro) = CFGstmt;
					}
					if (prevCon == 2) {
						CFGline.push_back(dummy);
						CFGstmt.push_back(CFGline);
						CFGTable.at(currentPro) = CFGstmt;
					}
					else {
						if (parentForElseIf != -1) {
							CFGline = CFGTable.at(currentPro).at(parentForElseIf);
							CFGline.push_back(stmtNo);
							CFGstmt.at(parentForElseIf) = CFGline;
							CFGTable.at(currentPro) = CFGstmt;
						}
						else {
							CFGline.push_back(dummy);
							CFGstmt.push_back(CFGline);
							CFGTable.at(currentPro) = CFGstmt;
						}

					}

				}

			}
			else if (conditionstmt == 0 && stmt.size() != endloop) {
				if (closingStack.size() != 0) {
					int prev = closingStack.top();
					closingStack.pop();
					int con = clostingCondition.top();
					clostingCondition.pop();
					if (con == 3) {
						CFGline = CFGTable.at(currentPro).at(prev);
						CFGline.push_back(stmtNo);
						CFGstmt.at(prev) = CFGline;
						CFGTable.at(currentPro) = CFGstmt;
					}
					else {
						if (!flagForCorrectElseIf) {
							CFGline.push_back(dummy);
							CFGstmt.push_back(CFGline);
							CFGTable.at(currentPro) = CFGstmt;
						}
					}
				}
			}
			if (!flag) {
				flagForClose = false;
			}
			else {
				flagForClose = true;
				flag = false;
			}
		}
		else {
			if (conditionstmt == 0 && stmt.size() != endloop && flagForNewProc == false) {
				CFGline.push_back(stmtNo);
				CFGstmt.push_back(CFGline);
				CFGTable.at(currentPro) = CFGstmt;

			}
			else if (conditionstmt == 1) {
				if (!flagForNewProc) {
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
					CFGline.push_back(stmtNo);
					CFGstmt.push_back(CFGline);
					CFGTable.at(currentPro) = CFGstmt;
				}
				else {
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
				}
			}
			else if (conditionstmt == 2) {
				int parent = parentStack.top();
				parentStack.push(parent);
				conditionStack.push(conditionstmt);
			}
			else if (conditionstmt == 3) {
				if (!flagForNewProc) {
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);
					CFGline.push_back(stmtNo);
					CFGstmt.push_back(CFGline);
					CFGTable.at(currentPro) = CFGstmt;
				}
				else {
					parentStack.push(stmtNo);
					conditionStack.push(conditionstmt);

				}

			}
		}
		flagForNewProc = false;
		if (endloop > 0) {
			int currentNo;
			flagForClose = true;
			for (int i = 0; i < endloop; i++) {
				if (parentStack.size() == 0) {
					while (closingStack.size() != 0) {
						closingStack.pop();
						clostingCondition.pop();
					}
					while (ifRecord.size() != 0) {
						ifRecord.pop();
					}
					flagForClose = false;
					flagForCorrectElseIf = false;
					flag = false;
					flagNextLevelStart = false;
					break;
				}
				if (parentStack.size() > 0 && conditionStack.size() > 0) {
					int parent = parentStack.top();
					parentStack.pop();
					int con = conditionStack.top();
					conditionStack.pop();
					int index = CFGstmt.size();
					if (con == 3) {
						if (closingStack.size() == 0) {
							currentNo = stmtNo;
						}
						else if (clostingCondition.top() != 3) {
							currentNo = stmtNo;
							closingStack.pop();
							clostingCondition.pop();
						}
						else {
							if (closingStack.size() == 0) {

							}
							else {
								currentNo = closingStack.top();
								closingStack.pop();
								clostingCondition.pop();
							}

						}
						closingStack.push(parent);
						clostingCondition.push(con);
						if (index == stmtNo) {
							CFGline.push_back(parent);
							CFGstmt.push_back(CFGline);
							CFGTable.at(currentPro) = CFGstmt;
						}
						else {

							if (currentNo >= CFGstmt.size()) {

							}
							else {
								CFGline = CFGstmt.at(currentNo);
								CFGline.push_back(parent);
								CFGstmt.at(currentNo) = CFGline;
								CFGTable.at(currentPro) = CFGstmt;
							}

						}
					}
					else if (con == 2) {
						closingStack.push(stmtNo);
						clostingCondition.push(con);
						CFGline = CFGTable.at(currentPro).at(parent);
						CFGline.push_back(stmtNo + 1);
						CFGstmt.at(parent) = CFGline;
						CFGTable.at(currentPro) = CFGstmt;
					}
					else if (con == 1) {
						if (!flagForCorrectElseIf) {

							CFGline.push_back(dummy);
							CFGstmt.push_back(CFGline);
							CFGTable.at(currentPro) = CFGstmt;
							ifRecord.push(parent);
						}
						else {
							CFGline = CFGTable.at(currentPro).at(parent);
							if (CFGline.size() == 2 && (CFGline.at(0) != -1 && CFGline.at(1) != -1)) {
								flagForCorrectElseIf = false;
							}
							else {
								CFGline.push_back(stmtNo);
								CFGstmt.at(parent) = CFGline;
								CFGTable.at(currentPro) = CFGstmt;
								flagForCorrectElseIf = false;
							}

						}

						closingStack.push(stmtNo);
						clostingCondition.push(con);


					}
				}

			}
		}
	}
	catch (exception &e) {
		//	cout << "Standard exception (for CFG): " << e.what() << endl;
	}
	*/
}

void CFG::reverCFG() {

	vector<vector<int>> temp = CFGTable.at(currentPro);
	vector<vector<int>> arr(numOfStatement + 2);
	vector<int> prevRecord;
	vector<int> ::iterator ite;
	for (int i = 0; i < temp.size(); i++) {
		for (ite = temp.at(i).begin(); ite != temp.at(i).end(); ite++) {
			if (*ite == -1) {
				prevRecord = arr[numOfStatement + 1];
				prevRecord.push_back(i);
				arr[numOfStatement + 1] = prevRecord;

			}
			else {
				prevRecord = arr[*ite];
				prevRecord.push_back(i);
				arr[*ite] = prevRecord;
			}

		}
	}
	revCFGstmt = arr;
}

int setConditions(string stmtLine) {
	if (stmtLine.find("if") != std::string::npos) {
		conditionstmt = 1;

	}
	else if (stmtLine.find("else") != std::string::npos) {
		conditionstmt = 2;

	}
	else if (stmtLine.find("while") != std::string::npos) {
		conditionstmt = 3;

	}
	else {
		conditionstmt = 0;
	}
	return conditionstmt;
}

string trimString(string stmt) {
	stmt.erase(remove(stmt.begin(), stmt.end(), '\t'), stmt.end());
	size_t endpos = stmt.find_last_not_of(" ");
	if (string::npos != endpos) {
		stmt = stmt.substr(0, endpos + 1);
	}
	size_t startpos = stmt.find_first_not_of(" ");
	if (string::npos != endpos) {
		stmt = stmt.substr(startpos);
	}
	return stmt;
}


vector<int> CFG::getNext(int stmtNo) {

	vector<int> result;
	if (stmtNo > numOfStatement) {
		return result;
	}
	vector<int> temp;
	temp = CFGTable.at(currentPro).at(stmtNo);
	for (int i = 0; i < temp.size(); i++) {

		if (temp.at(i) == -1) {
			return result;
		}
		result.push_back(temp.at(i));
		//		}
	}
	return result;

}


vector<int> CFG::getPrev(int stmtNo) {

	vector<int>result;
	if (stmtNo > numOfStatement) {
		return result;
	}
	vector<int>temp = revCFGstmt.at(stmtNo);
	result = temp;
	return result;

}


bool CFG::isNext(int stmtNo1, int stmtNo2) {
	if (stmtNo1 > numOfStatement || stmtNo2 > numOfStatement) {
		return false;
	}
	vector<int> temp;
	temp = CFGTable.at(currentPro).at(stmtNo1);
	for (int i = 0; i < temp.size(); i++) {
		if (temp.at(i) == stmtNo2) {
			return true;
		}
	}
	return false;

}


bool CFG::isNextStar(int s1, int s2) {
	if (s1 > numOfStatement || s2 > numOfStatement) {
		return false;
	}
	bool* visited = new bool[CFGTable.at(currentPro).size()]();
	stack <int> stack;
	bool ans = false;
	bool goBackItSelf = false;
	stack.push(s1);
	vector<int> ::iterator it;

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		visited[top] = true;
		for (it = CFGTable.at(currentPro).at(top).begin(); it != CFGTable.at(currentPro).at(top).end(); it++) {

			if (*it == s2) {
				if (goBackItSelf == false) {
					goBackItSelf = true;
					return true;
				}
			}
			if (!visited[*it]) {
				stack.push(*it);
				if (*it == s2) {
					ans = true;
					return ans;
				}
			}
		}
	}
	return ans;

}

vector<int> CFG::getNextStar(int stmtNo) {

	bool* visited = new bool[CFGTable.at(currentPro).size()]();
	stack <int> stack;
	bool goBackItSelf = false;
	vector<int> ans;
	if (stmtNo > numOfStatement) {
		return ans;
	}
	stack.push(stmtNo);
	vector<int> ::iterator it;

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		visited[top] = true;
		for (it = CFGTable.at(currentPro).at(top).begin(); it != CFGTable.at(currentPro).at(top).end(); it++) {
			if (*it == stmtNo) {
				if (goBackItSelf == false) {
					ans.push_back(*it);
					goBackItSelf = true;
				}
			}
			if (!visited[*it]) {
				stack.push(*it);
				ans.push_back(*it);
			}
		}
	}
	return ans;
}

vector<int> CFG::getPrevStar(int stmtNo) {
	bool* visited = new bool[CFGTable.at(currentPro).size()]();
	stack <int> stack;
	vector<int> ans;
	bool goBackItSelf = false;
	if (stmtNo > numOfStatement) {
		return ans;
	}
	stack.push(stmtNo);
	vector<int> ::iterator it;

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		visited[top] = true;
		for (it = revCFGstmt.at(top).begin(); it != revCFGstmt.at(top).end(); it++) {
			if (*it == stmtNo) {
				if (goBackItSelf == false) {
					ans.push_back(*it);
					goBackItSelf = true;
				}
			}
			if (!visited[*it]) {
				stack.push(*it);
				if (*it != 0) {
					ans.push_back(*it);
				}

			}
		}
	}
	return ans;
}


vector<string> CFG::getNextString(int stmtNo) {

	vector<string> result;
	if (stmtNo > numOfStatement) {
		return result;
	}
	vector<int> temp;
	temp = CFGTable.at(currentPro).at(stmtNo);
	for (int i = 0; i < temp.size(); i++) {
		//		if (checkProcedure(stmtNo, temp.at(i))) {
		if (temp.at(i) == -1) {
			return result;
		}
		result.push_back(to_string(temp.at(i)));
		//		}
	}
	return result;

}


vector<string> CFG::getPrevString(int stmtNo) {

	vector<string>result;
	if (stmtNo > numOfStatement) {
		return result;
	}
	vector<int>temp = revCFGstmt.at(stmtNo);
	for (int i = 0; i < temp.size(); i++) {
		result.push_back(to_string(temp.at(i)));
	}
	return result;
}


vector<string> CFG::getNextStarString(int stmtNo) {
	bool* visited = new bool[CFGTable.at(currentPro).size()]();
	stack <int> stack;
	bool goBackItSelf = false;
	vector<string> ans;
	if (stmtNo > numOfStatement) {
		return ans;
	}
	stack.push(stmtNo);
	vector<int> ::iterator it;

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		visited[top] = true;
		for (it = CFGTable.at(currentPro).at(top).begin(); it != CFGTable.at(currentPro).at(top).end(); it++) {
			if (*it == stmtNo) {
				if (goBackItSelf == false) {
					ans.push_back(to_string(*it));
					goBackItSelf = true;
				}
			}
			if (!visited[*it]) {
				stack.push(*it);
				ans.push_back(to_string(*it));
			}
		}
	}
	return ans;
}


vector<string> CFG::getPrevStarString(int stmtNo) {
	bool* visited = new bool[CFGTable.at(currentPro).size()]();
	stack <int> stack;
	bool goBackItSelf = false;
	vector<string> ans;
	if (stmtNo > numOfStatement) {
		return ans;
	}
	stack.push(stmtNo);
	vector<int> ::iterator it;

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		visited[top] = true;
		for (it = revCFGstmt.at(top).begin(); it != revCFGstmt.at(top).end(); it++) {
			if (*it == stmtNo) {
				if (goBackItSelf == false) {
					ans.push_back(to_string(*it));
					goBackItSelf = true;
				}
			}

			if (!visited[*it]) {
				stack.push(*it);
				if (*it != 0) {
					ans.push_back(to_string(*it));
				}

			}
		}
	}
	return ans;
}




