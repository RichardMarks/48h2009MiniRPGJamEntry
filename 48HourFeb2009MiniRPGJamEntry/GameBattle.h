
// CODESTYLE: v2.0

// GameBattle.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: The battle (encounter/fighting) engine

#ifndef __GAMEBATTLE_H__
#define __GAMEBATTLE_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
	class BitmapFont;
}

namespace GAME
{
	using ENGINE::ImageResource;
	using ENGINE::BitmapFont;

	namespace BATTLE
	{
		/// the states the battle system can be in
		enum BattleState
		{
			/// the battle system should initialize the combatants, the scenery, and timers
			Initialize,

			/// the battle system is currently running, and the battle logic should be processed
			InBattle,

			/// the battle system is ready to finish, pointers for timers, and battle resources should be released
			EndBattle,

			/// the battle system is finished, and game play will resume
			Finished
		};

		/// the types of rewards you can gain from battle
		enum RewardType
		{
			/// the player receives a sum of money
			CashReward,

			/// player receives an item
			ItemReward,

			/// player receives a new skill
			SkillReward,

			/// player receives a skill upgrade
			SkillUpgradeReward,

			/// player recieves battle experience points
			ExperienceReward
		};

/*#**************************************************************************#*/

		/// a class for handling battle rewards
		class Reward
		{
		public:
			Reward();
			~Reward();

			/// inits the reward
			void Set(RewardType rewardType, unsigned int amount, int itemIndex = -1, int skillIndex = -1);

			/// gets the reward type
			RewardType GetRewardType() const;

			/// gets the amount of the reward
			unsigned long GetAmount() const;

			/// gets the item index
			int GetItemIndex() const;

			/// gets the skill index
			int GetSkillIndex() const;

		private:
			Reward(const Reward& rhs);
			const Reward& operator=(const Reward& rhs);

		private:
			/// the type of reward
			RewardType rewardType_;

			/// the amount of the reward
			unsigned long amount_;

			/// the item index used only if its an item reward
			int itemIndex_;

			/// the skill index used only if its a skill or skill upgrade reward
			int skillIndex_;
		};

/*#**************************************************************************#*/

		/// a class to hold battle data for each combatant
		class Combatant
		{
		public:
			Combatant();
			~Combatant();

		private:
			Combatant(const Combatant& rhs);
			const Combatant& operator=(const Combatant& rhs);

		private:
			// TODO: Add combatant data members
		};
	} // end namespace

/*#**************************************************************************#*/


	class GameBattleSpriteManager;
	class GameBattleSceneManager;

	/// the battle system singleton class
	class BattleEngineSingleton
	{
	public:
		static BattleEngineSingleton* GetInstance();
		~BattleEngineSingleton();

		/// initialize the battle system
		bool Initialize();

		/// clean up after the battle system
		void Destroy();

		/// update the battle system
		void Update();

		/// render the battle system
		void Render();

		/// changes the battle system state
		void SetState(BATTLE::BattleState state);

		/// gets the battle system state
		BATTLE::BattleState GetState() const;

	private:
		BattleEngineSingleton();
		BattleEngineSingleton(const BattleEngineSingleton& rhs);
		const BattleEngineSingleton& operator=(const BattleEngineSingleton& rhs);

	private:

		/// initialies the battle system resources -- called in Update when @state_ is BATTLE::Initialize
		void InitializeBattleResources();

		/// cleans up the battle system resources -- called in Update when @state_ is BATTLE::EndBattle
		void DestroyBattleResources();

		/// loads files of the the .battle format to initialize the battle system
		void LoadBattleFile(const char* battlePath);

		/// the state of the battle system
		BATTLE::BattleState state_;

		/// holds the list of sprite file names
		std::vector<std::string> spriteFilenames_;

		/// manages the sprites that are available to the battle system
		GameBattleSpriteManager* battleSprites_;

		/// manages the scenes that are available to the battle system
		GameBattleSceneManager* battleScenes_;

		// shared resource pointers -- the game singleton class is responsible for init/destroy these pointers

		/// shared resource pointer to the small font of the game system
		BitmapFont* smallFont_;

		/// shared resource pointer to the large font of the game system
		BitmapFont* largeFont_;

		/// shared resource pointer to the display surface of the game system
		ImageResource* microDisplay_;

		// battle system resource pointers -- the battle system is responsible for init/destroy these pointers

		/// the background scene image of the battle
		ImageResource* battleSceneImage_;

		/// the cool looking borders are really just an image overlaid
		ImageResource* windowOverlay_;

		/// the battle menu cursor
		ImageResource* cursorOverlay_;

		/// the borders for the battle gauge that shows the order of the combatants' turns
		ImageResource* battleGaugeOverlay_;


		/// the list of combatants
		std::vector<BATTLE::Combatant*> combatants_;

		/// the battle rewards that are awarded to the player if the player is alive when the battle ends
		std::vector<BATTLE::Reward*> rewards_;

	}; // end class

	/// simple define to access the singleton class pointer easily
	#define BattleEngine BattleEngineSingleton::GetInstance()
} // end namespace
#endif


