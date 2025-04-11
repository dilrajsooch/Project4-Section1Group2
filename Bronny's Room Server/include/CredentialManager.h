#pragma once
#include <string>

bool ValidateCredentials(const std::string& username, const std::string& password);

bool RegisterCredentials(const std::string& username, const std::string& password);

int AuthenticateUser(const std::string& username, const std::string& password);

int RegisterUser(const std::string& username, const std::string& password);

std::string GetAccountID(const std::string& username);