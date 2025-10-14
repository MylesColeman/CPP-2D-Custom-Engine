#pragma once
#include <vector>

// Enum representing all possible actions that can be taken
enum class Actions
{
	eNone,
	eMoveUp,
	eMoveRight,
	eMoveDown,
	eMoveLeft
};

class IReceivesInput
{
public:
	virtual void HandleInput(const std::vector<Actions>& action) = 0; // This function takes a vector of actions to handle multiple inputs at once
};