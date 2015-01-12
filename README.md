# koto
"koto" is a header only string library for C++
## 概要
std::stringよりもちょっと効率が良くて、エンコードする情報を持った文字列クラスライブラリ(を目指しているつもり)。趣味で書いているのでやる気があるときだけ作ってます。つまりほとんど実装できていません。また、std::stringとの互換性はありません。
## サポート環境
C++03をサポートしているコンパイラで使用できます。開発は以下の環境で行っています。

 * Xcode 6.1.1(Mac OS X 10.9.4)

## 使い方
### 普通の使い方
ヘッダオンリーなので、「koto/koto.hpp」をインクルードすれば使えます。

	#include "koto/koto.hpp"

文字コードはデフォルトでUTF8に設定されています。

	using namespace koto;
	string a = "ABCあいう";
	std::cout << a.length() << std::endl; // 6
	std::cout << a.size() << std::endl;   // 12

`koto::string`は効率化のためCopy on Writeな実装になっています(するつもりです)。  
固定長文字列を扱う`koto::fixed<S>::string`もあります。`koto::string`に比べてできることが制限されますが、スタック上にメモリを確保するので高速です。こちらはCopy on Writeではありません。
### エンコーディングを利用する
`koto::string`は以下のように定義されています。

	typedef basic_string<char, encoding_utf8<char>::static_type > string;

このクラスはUTF8を扱う文字列クラスです。他のエンコーディングの文字列を代入することは出来ません。

	typedef basic_string<char, encoding_ascii<char>::static_type > string;

のように定義することで、ASCIIコードを使用する文字列を定義できます。また、動的エンコーディングを利用することで、プログラム中に任意のエンコーディングに切り替えることが出来ます(**別のエンコーディングへの変換はサポートしていません!**)。

	using namespace koto;
	typedef basic_string<char, dynamic_encoding<char> > string;

	{
		string str = "ABCあいう";
		std::cout << str.length() << "\n"; // 6
	}

	encoding<char> *enc = new encoding_ascii<char>::dynamic_type();
	string::set_default_encoding(enc);

	{
		string str = "ABCあいう";
		std::cout << str.length() << "\n"; // 12
	}

	delete enc;

動的エンコーディングを利用した場合、実行時に文字コード別処理が判別されるため、静的エンコーディングを利用した場合に比べ若干遅くなります(気になるレベルではないと思いますが…)。  
定義するのが面倒な人は、"koto.hpp"をインクルードする前に

	#define KOTO_DEFAULT_ENCODING ascii

と書くと、その文字コードがデフォルトの文字コードとなります。サポートしているのは「utf8」と「ascii」のみです。

### その他
sampleフォルダにサンプルが入っているのでそちらを参照してください。

---
koto is distributed under Boost Software License 1.0. See License.txt