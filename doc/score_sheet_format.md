# 棋譜のフォーマットについて

カルカソンヌの棋譜を保存するのに利用するファイル形式と棋譜の表現方法について述べる。

## ファイル形式

保存するファイル形式としてJSONを採用する。

理由は以下の通り

- 人間でも読み書きしやすい
- 多くのプログラミング言語で手軽に扱える
- 必要十分な表現力を持っている


## 棋譜の表現方法

カルカソンヌの棋譜はこれから定義するSCORE_SHEETというobjectで表現される。([棋譜の例](./championship_japan_2015_final.json))


### SCORE_SHEET

SCORE_SHEETはひとつの試合に関する棋譜の情報を表す。

- 定義
  - SCORE_SHEETは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "info" | 必須 | object | 試合の情報を表す[INFO](#info)という形式のobjectが値にくる。 |
| "placements" | 必須 | object | 手順の情報を表す[PLACEMENTS](#placements)という形式のarrayが値にくる。 | 

### INFO

INFOはその棋譜の元となった試合の情報を表す。

- 定義
  - INFOは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "title" | 必須 | string | 試合名を表すstringが値にくる |
| "startDate" | オプショナル | string | 試合の開始時刻を表す[ISO 8601](https://ja.wikipedia.org/wiki/ISO_8601)形式のstringが値にくる。 |
| "endDate" | オプショナル | string | 試合の終了時刻を表す[ISO 8601](https://ja.wikipedia.org/wiki/ISO_8601)形式のstringが値にくる。 |
| "players" | 必須 | array | 試合に参加したプレイヤーを表す[PLAYER](#player)を含むarrayが値にくる。array内での[PLAYER](#player)の位置は自由である。 |

### PLAYER

試合に参加したプレイヤーに関する情報を表す。

- 定義
  - PLAYERは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "name" | 必須 | string | プレイヤー名を表すstringが値にくる。 |
| "color" | オプショナル | string | プレイヤーのミープルの色を表すstringが値にくる。 | 

- 他のPLAYERとの間にある制約
  - "name"フィールドの値が同じPLAYERが2つ存在してはならない
  - "color"フィールドの値が同じPLAYERが2つ存在してはならない

### PLACEMENTS

PLACEMENTSはタイルの配置やミープルの配置といった手順に関する情報を表す。  

- 定義
  - PLACEMENTSはarrayである
  - PLACEMENTSの要素は[PLACEMENT](#placement)である
    - 補足：後で述べるが[PLACEMENT](#placement)には[INITIAL_PLACEMENT](#initial_placement)、[SKIPPED_PLACEMENT](#skipped_placement)、[REGULAR_PLACEMENT](#regular_placement)の3種類ある
  - PLACEMENTSの要素数に決まりはない
  - PLACEMENTS内の要素の順番には意味があり、最初の要素ほど最初の手順を表す
  - PLACEMENTSの最初の要素は[INITIAL_PLACEMENT](#initial_placement)でなければならない
  - [INITIAL_PLACEMENT](#initial_placement)はPLACEMENTSの最初以外の要素であってはいけない

### PLACEMENT

PLACEMENTはひとつの手番におけるタイルの配置に関する情報を表す。  
ミープルの配置を伴う場合はミープルの配置に関する情報も含む。

PLACEMENTは配置のタイプによって3つの種類がある。(それぞれで微妙に定義が違う。)

- [INITIAL_PLACEMENT](#initial_placement)
- [SKIPPED_PLACEMENT](#skipped_placement)
- [REGULAR_PLACEMENT](#regular_placement)


#### INITIAL_PLACEMENT

INITIAL_PLACEMENTはタイルの配置の中でもゲーム開始時に予め配置されているタイルを表現するための配置情報を表す。  
カルカソンヌの通常のルールでは最初に配置されるタイルの種類は固定なので、このような情報がなくても問題ないが、予め配置されているタイルの種類やその配置方法を明示的にするために設けた。  

- 定義
  - INITIAL_PLACEMENTは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "type" | 必須 | string | "initial"というstringが値にくる。 |
| "tile" | 必須 | string | タイルの種類を表すstringが値にくる。 | 
| "tilePlacement" | オプショナル | object | タイルの配置方法を表す[TILE_PLACEMENT](#tile_placement)という形式の値がくる。省略された場合は全てのフィールドがデフォルト値の[TILE_PLACEMENT](#tile_placement)が指定されたものと解釈される。 | 

#### SKIPPED_PLACEMENT

SKIPPED_PLACEMENTはタイルを配置できる場所がなくスキップした場合の配置情報を表す。

- 定義
  - SKIPPED_PLACEMENTは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "type" | 必須 | string | "skipped"というstringが値にくる。 |
| "tile" | 必須 | string | タイルの種類を表すstringが値にくる。 | 
| "player" | 必須 | string | どのプレイヤーの手番でスキップが発生したかを表すためのプレイヤー名を表すstringがくる。このプレイヤー名は[INFO](#info)のプレイヤー情報で出てきたプレイヤー名でなければならない。 |

#### REGULAR_PLACEMENT

REGULAR_PLACEMENTはプレイヤーによる通常のタイル配置を表す。

- 定義
  - REGULAR_PLACEMENTは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "type" | 必須 | string | "regular"というstringが値にくる。 |
| "tile" | 必須 | string | タイルの種類を表すstringが値にくる。 | 
| "tilePlacement" | 必須 | object | タイルの配置方法を表す[TILE_PLACEMENT](#tile_placement)という形式の値がくる。 | 
| "meeplePlacement" | オプショナル | object | ミープルの配置方法を表す[MEEPLE_PLACEMENT](#meeple_placement)という形式の値がくる。ミープルを配置しなかった手番では不要。 |

### TILE_PLACEMENT

タイルの配置方法に関する情報を表す。

- 定義
  - TILE_PLACEMENTは以下の表のようなobjectである

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "x" | オプショナル | int | タイルを配置するx座標を表すintが値にくる。省略された場合は0が指定されたと解釈される。 |
| "y" | オプショナル | int | タイルを配置するy座標を表すintが値にくる。省略された場合は0が指定されたと解釈される。 |
| "rotation" | オプショナル | int | タイルを配置する際にタイルを時計回りに何回転させたかを表すintが値にくる。0から3の数字でなければならない。省略された場合は0が指定されたと解釈される。 |

- [INITIAL_PLACEMENT](#initial_placement)中のTILE_PLACEMENTに適用される制限
  - "x"フィールドにどのような値が指定されていても強制的に0が指定されているものと解釈される
  - "y"フィールドにどのような値が指定されていても強制的に0が指定されているものと解釈される

### MEEPLE_PLACEMENT

ミープルの配置方法に関する情報を表す。

| フィールド名 | 必須か | 値の型 | 説明 |
|---|---|---|---|
| "segmentType" | 必須 | string | ミープルを配置したセグメントのタイプを表すstringが値にくる。セグメントのタイプは"city"、"road"、"cloister"、"field"のいずれか。|
| "segmentIndex" | 必須 | int | ミープルを配置したセグメントのインデックスを表すintが値にくる。|

### その他

#### タイルの種類について

カルカソンヌでは様々な種類のタイルがある。  
タイルの種類を区別するためにタイルの種類毎に名前を付ける。  
タイルの種類の名前はカルカソンヌの説明書に書かれている名前をそのまま採用する。  
例えば「道付き修道院」の名前は"A"、「修道院」の名前は"B"である。
"A"から"X"までが標準のカルカソンヌで使われるタイルである。

#### タイルの座標について

タイルを配置する場所は2次元座標(x座標とy座標)で表す。

- 最初に配置したタイルの座標を(x, y) = (0, 0)とする
- 最初に配置したタイルの上に配置したタイルの座標は(x, y) = (0, 1)になる
- 最初に配置したタイルの右に配置したタイルの座標は(x, y) = (1, 0)になる
- 最初に配置したタイルの下に配置したタイルの座標は(x, y) = (0, -1)になる
- 最初に配置したタイルの左に配置したタイルの座標は(x, y) = (-1, 0)になる
