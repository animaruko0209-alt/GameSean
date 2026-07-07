// ConsoleApplication1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <conio.h>   // _getch
#include <cstdlib>   // system("cls")
#include <random>
#include <thread>
#include <chrono>

class GameManager {

private:
	/// <summary>
	/// オブジェクトを外部から生成するのを防ぐためのprivateコンストラクタ
	/// </summary>
	enum class State { Title, Screen, GameOver, GameClear };
	State currentState;

	GameManager() : currentState(State::Title) {}

	// 乱数生成器
	std::mt19937 rng{ static_cast<unsigned int>(
		std::chrono::high_resolution_clock::now().time_since_epoch().count()) };

public:

	/// <summary>
	/// 別のオブジェクトのコピーを禁止するためのコピーコンストラクタ
	/// </summary>
	/// <param name=""></param>
	GameManager(const GameManager&) = delete;
	/// <summary>
	/// 別のオブジェクトのコピーを禁止するためのコピーコンストラクタ
	/// </summary>
	/// <param name=""></param>

	GameManager& operator=(const GameManager&) = delete;

	/// <summary>
	/// スレッドセーフな、GameManagerの唯一のオブジェクトを返すことができる
	/// </summary>
	/// <returns></returns>
	static GameManager& Instance() {
		static GameManager instance;
		return instance;
	}


	void GameTitle() {
		std::cout << "===== タイトル =====" << std::endl;
		std::cout << "ゲームへようこそ！" << std::endl;
		std::cout << "何かキーを押すとゲーム開始（qで終了）" << std::endl;
	}

	void GameScreen() {
		std::cout << "===== ゲーム画面 =====" << std::endl;
		std::cout << "判定（30%で当たり）" << std::endl;
		std::cout << "キーを押してください（qで終了）" << std::endl;
	}

	void GameOver() {
		std::cout << "===== ゲームオーバー =====" << std::endl;
		std::cout << "はずれ" << std::endl;
		std::cout << "キーを押してください。" << std::endl;
	}

	void GameClear() {
		std::cout << "===== ゲームクリア =====" << std::endl;
		std::cout << "当たり" << std::endl;
		std::cout << "キーを押してください。" << std::endl;
	}

	// 現在の状態に応じて画面表示
	void DisplayCurrent() {
		switch (currentState) {
		case State::Title:
			GameTitle();
			break;
		case State::Screen:
			GameScreen();
			break;
		case State::GameOver:
			GameOver();
			break;
		case State::GameClear:
			GameClear();
			break;
		}
	}

	// ゲーム画面での処理：キー入力で判定（qで終了）
	// true を返すと終了要求
	bool PlayOnce() {
		currentState = State::Screen;
		system("cls");
		DisplayCurrent();

		int c = _getch(); // 判定用のキー待ち（Enter不要）
		if (c == 'q' || c == 'Q') {
			return true; // 終了要求
		}

		std::cout << "\n判定中";
		// 簡易な処理アニメーション
		for (int i = 0; i < 6; ++i) {
			std::cout << ".";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::cout << std::endl;

		// 0..99 の乱数を生成して30%未満を当たりとする
		std::uniform_int_distribution<int> dist(0, 99);
		int value = dist(rng);

		if (value < 30) {
			currentState = State::GameClear;
		}
		else {
			currentState = State::GameOver;
		}

		return false;
	}

	// タイトルへ戻す
	void ReturnToTitle() {
		currentState = State::Title;
	}
};

int main() {
	auto& gm = GameManager::Instance();

	while (true) {
		system("cls"); // コンソールをクリア（Windows向け）
		gm.DisplayCurrent();
		std::cout << "\nキーを押してください（qで終了）..." << std::endl;

		int c = _getch(); // キーを1つ待つ（Enter不要）
		if (c == 'q' || c == 'Q') {
			break;
		}

		// タイトルからゲーム実行へ（PlayOnce が true を返したら終了）
		if (gm.PlayOnce()) {
			break;
		}

		// 結果表示（GameClearまたはGameOver）
		system("cls");
		gm.DisplayCurrent();

		// 結果画面でキー待ち：qで終了、それ以外でタイトルへ戻る
		int r = _getch();
		if (r == 'q' || r == 'Q') {
			break;
		}

		gm.ReturnToTitle();
	}

	return 0;
}



