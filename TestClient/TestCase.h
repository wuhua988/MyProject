#ifndef __TestCase_h__
#define __TestCase_h__
#include <string>
#include <vector>

void TestUserAuth(const std::vector<std::string>& args);
void TestGetUserInfo(const std::vector<std::string>& args);
void TestGetFriendInfo(const std::vector<std::string>& args);
void TestGetFriendList(const std::vector<std::string>& args);
void TestQuitUser(const std::vector<std::string>& args);
void TestUpdateUserInfo(const std::vector<std::string>& args);
void TestChgUserInfo(const std::vector<std::string>& args);
void TestGetUserState(const std::vector<std::string>& args);
#endif //__TestCase_h__