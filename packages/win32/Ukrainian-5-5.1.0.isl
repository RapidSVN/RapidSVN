; *** Inno Setup version 5.1.0+ Ukrainian messages ***
;
; Translation was made by Oleh Malyy 
; mals@dreamsoft.ssft.net 
; http://www.dreamsoft-sg.com/
;
; Based on translation by Vitaliy Levchenko v.4.0.5
; Edited by Olha Honcharova
;


[LangOptions]
LanguageName=Ukrainian
LanguageID=$0422
LanguageCodePage=1251

[Messages]

; *** Application titles
SetupAppTitle=Інсталяція
SetupWindowTitle=Інсталяція - %1
UninstallAppTitle=Деінсталяція
UninstallAppFullTitle=Деінсталяція %1

; *** Misc. common
InformationTitle=Інформація
ConfirmTitle=Увага
ErrorTitle=Помилка

; *** SetupLdr messages
SetupLdrStartupMessage=Інсталяція %1. Продовжити?
LdrCannotCreateTemp=Не можу створити тимчасовий файл. Інсталяція перервана
LdrCannotExecTemp=Не можу виконати файл у каталозі тимчасових файлів. Інсталяція перервана

; *** Startup error messages
LastErrorMessage=%1.%n%nПомилка %2: %3
SetupFileMissing=В інсталяційному каталозі відсутній файл %1 . Будь ласка, виправте цю помилку або візьміть іншу копію програми.
SetupFileCorrupt=Файли інсталяції пошкоджені. Будь ласка, візьміть іншу копію програми.
SetupFileCorruptOrWrongVer=Файли інсталяції пошкоджені або не сумісні з цією версією інсталяції. Будь ласка, виправте цю помилку або візьміть іншу копію програми.
NotOnThisPlatform=Ця програма не буде працювати під %1.
OnlyOnThisPlatform=Ця програма повинна запускатися під %1.
OnlyOnTheseArchitectures=Інсталяція цієї програми можлива тільки для версій Windows для наступних архітектур процесорів %n%n%1
MissingWOW64APIs=У версії Windows, в якій Ви працюєте, відсутні функції для виконання 64-розрядної інсталяції. Щоб позбутися цієї проблеми, Вам необхідно встановити пакет оновлення [Service Pack] %1
WinVersionTooLowError=Ця програма вимагає %1 версію %2 або пізнішу.
WinVersionTooHighError=Ця програма не може бути встановлена на %1 версію %2 або пізнішу.
AdminPrivilegesRequired=Ви повинні увійти в якості адміністратора для інсталяції цієї програми.
PowerUserPrivilegesRequired=Щоб інсталювати цю програму, ви повинні увійти в якості адміністратора або члена адміністративної групи
SetupAppRunningError=Виявлено, що зараз запущено %1.%n%nБудь ласка, закрийте всі її копії і натисніть OK для продовження або «Скасувати» для виходу.
UninstallAppRunningError=Виявлено, що зараз запущено %1.%n%nБудь ласка, закрийте всі її копії і натисніть OK для продовження або «Скасувати» для виходу.

; *** Misc. errors
ErrorCreatingDir=Програма інсталяції не змогла створити каталог "%1"
ErrorTooManyFilesInDir=Неможливо створити файл у каталозі "%1", бо в ньому забагато файлів

; *** Setup common messages
ExitSetupTitle=Вихід з інсталяції
ExitSetupMessage=Інсталяцію не завершено. Якщо Ви вийдете зараз, програму не буде встановлено.%n%nВи повинні будете запустити програму інсталяції іншим разом для закінчення.%n%nВийти з програми інсталяції?
AboutSetupMenuItem=&Про програму інсталяції...
AboutSetupTitle=Про програму інсталяції
AboutSetupMessage=%1 версія %2%n%3%n%n%1 домашня сторінка:%n%4
AboutSetupNote=
TranslatorNote=Ukrainian translation by Oleh Malyy, http://www.dreamsoft-sg.com

; *** Buttons
ButtonBack=< &Назад
ButtonNext=&Далі >
ButtonInstall=&Встановити
ButtonOK=OK
ButtonCancel=Скасувати
ButtonYes=&Так
ButtonYesToAll=Так для &всіх
ButtonNo=&Ні
ButtonNoToAll=Н&і для всіх
ButtonFinish=&Готово
ButtonBrowse=&Перегляд...
ButtonWizardBrowse=&Перегляд
ButtonNewFolder=&Створити новий каталог

; *** "Select Language" dialog messages
SelectLanguageTitle=Виберіть мову інсталяції
SelectLanguageLabel=Виберіть мову, яка буде використовуватися під час інсталяції:

; *** Common wizard text
ClickNext=Натисніть «Далі» для продовження, «Скасувати» для виходу.
BeveledLabel=
BrowseDialogTitle=Перегляд каталогів
BrowseDialogLabel=Виберіть каталог із списку і натисніть «ОК».
NewFolderName=Новий каталог

; *** "Welcome" wizard page
WelcomeLabel1=Ласкаво просимо до програми інсталяції [name].
WelcomeLabel2=Буде інсталюватися [name/ver] на вашу машину.%n%nРекомендуємо закрити всі інші запущені програми перед продовженням для уникнення можливих конфліктів під час інсталяції.

; *** "Password" wizard page
WizardPassword=Пароль
PasswordLabel1=Ця інсталяція захищена паролем.
PasswordLabel3=Будь-ласка, вкажіть пароль, потім натисніть «Далі» для продовження. Пароль чутливий до регістру.
PasswordEditLabel=&Пароль:
IncorrectPassword=Ви ввели неправильний пароль. Будь ласка, спробуйте ще раз.

; *** "License Agreement" wizard page
WizardLicense=Ліцензійна домовленість
LicenseLabel=Будь ласка, прочитайте цю важливу інформацію перед продовженням.
LicenseLabel3=Будь ласка, прочитайте ліцензійну домовленість. Ви повинні погодитися з умовами цієї домовленості для продовження інсталяції.
LicenseAccepted=Я &приймаю цю домовленість
LicenseNotAccepted=Я &не приймаю цієї домовленості

; *** "Information" wizard pages
WizardInfoBefore=Інформація
InfoBeforeLabel=Будь ласка, прочитайте цю важливу інформацію перед продовженням.
InfoBeforeClickLabel=Якщо ви готові продовжити, натисніть «Далі».
WizardInfoAfter=Інформація
InfoAfterLabel=Будь ласка, прочитайте цю важливу інформацію перед продовженням.
InfoAfterClickLabel=Якщо ви готові продовжити, натисніть «Далі».

; *** "User Information" wizard page
WizardUserInfo=Інформація про користувача
UserInfoDesc=Будь ласка, введіть інформацію про себе
UserInfoName=&Ім’я користувача:
UserInfoOrg=&Організація:
UserInfoSerial=&Серійний номер:
UserInfoNameRequired=Ви повинні ввести ім'я.

; *** "Select Destination Location" wizard page
WizardSelectDir=Виберіть каталог для інсталяції
SelectDirDesc=Куди має бути встановлено [name]?
SelectDirLabel3=Програма інсталює [name] у наступний каталог.
SelectDirBrowseLabel=Натисніть «Далі», щоб продовжити. Якщо Ви хочете вибрати інший каталог, натисніть «Перегляд».
DiskSpaceMBLabel=Необхідно як мінімум [mb] Mб вільного дискового простору.
ToUNCPathname=Не можна інсталювати до UNC каталога. Якщо ви хочете інсталювати в мережу, необхідно підключити мережний диск.
InvalidPath=Ви повинні ввести повний шлях, включно з диском;; наприклад:%nC:\APP
InvalidDrive=Вибраного вами диска не існує. Будь ласка, виберіть інший.
DiskSpaceWarningTitle=Недостатньо дискового простору
DiskSpaceWarning=Необхідно як мінімум %1 Kб вільного місця для інсталяції, але на вибраному диску доступні тільки %2 KB.%n%nПродовжити, не зважаючи на це?
DirNameTooLong=Ім’я каталога/шлях до нього занадто довгий.
InvalidDirName=Вказане ім’я каталога неприпустиме.
BadDirName32=Ім'я каталога не може містити наступних символів (після двокрапки):%n%n%1
DirExistsTitle=Каталог вже існує
DirExists=Каталог:%n%n%1%n%nвже існує. Ви все одно хочете інсталювати у нього?
DirDoesntExistTitle=Каталог не існує
DirDoesntExist=Каталог:%n%n%1%n%nне існує. Створити його?

; *** "Select Components" wizard page
WizardSelectComponents=Вибір компонентів
SelectComponentsDesc=Які компоненти мають бути встановлені?
SelectComponentsLabel2=Виберіть компоненти для інсталяції;; зніміть позначки із компонентів, які Ви не хочете інсталювати. Натисніть «Далі» для продовження.
FullInstallation=Повна інсталяція
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Компактна інсталяція
CustomInstallation=Вибіркова інсталяція
NoUninstallWarningTitle=Компоненти існують
NoUninstallWarning=Виявлено, що наступні компоненти вже інстальовані на Вашу машину:%n%n%1%n%n-вибір цих компонентів не буде деінсталювати їх.%n%nПродовжити в будь-якому випадку?
ComponentSize1=%1 Kб
ComponentSize2=%1 Mб
ComponentsDiskSpaceMBLabel=Даний вибір вимагає як мінімум [mb] Mб дискового простору.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Вибір додаткових задач
SelectTasksDesc=Які додаткові задачі потрібно виконати?
SelectTasksLabel2=Виберіть додаткові задачі, які програма інсталяції [name] повинна виконати, потім натисніть «Далі».

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Виберіть програмну групу
SelectStartMenuFolderDesc=Де потрібно розмістити ярлики для програми?
SelectStartMenuFolderLabel3=Інсталяція створить ярлики програми у наступній папці меню «Пуск».
SelectStartMenuFolderBrowseLabel=Натисніть «Далі» для продовження. Якщо Ви хочете вибрати іншу папку, натисніть «Перегляд».
MustEnterGroupName=Ви повинні ввести назву групи.
GroupNameTooLong=Ім’я папки / шлях до неї занадто довгий.
InvalidGroupName=Вказане ім’я папки неприпустиме.
BadGroupName=Назва групи не може включати наступні символи:%n%n%1
NoProgramGroupCheck2=&Не створювати програмну групу

; *** "Ready to Install" wizard page
WizardReady=Готово до інсталяції
ReadyLabel1=Все готово до інсталяції [name] на Ваш комп'ютер.
ReadyLabel2a=Натисніть «Встановити» для продовження інсталяції або «Назад», якщо ви хочете переглянути або змінити деякі параметри.
ReadyLabel2b=Натисніть «Встановити» для продовження інсталяції.
ReadyMemoUserInfo=Дані про користувача:
ReadyMemoDir=Цільовий каталог:
ReadyMemoType=Тип інсталяції:
ReadyMemoComponents=Вибрані компоненти:
ReadyMemoGroup=Програмна група:
ReadyMemoTasks=Додаткові задачі:

; *** "Preparing to Install" wizard page
WizardPreparing=Підготовка до інсталяції
PreparingDesc=Програма встановлення готується інсталювати [name] на Ваш комп'ютер.
PreviousInstallNotCompleted=Інсталяцію/деінсталяцію попередньої програми не завершено. Вам потрібно перезавантажити вашу машину для завершення інсталяції.%n%nПісля перезавантаження запустіть інсталяцію повторно для завершення встановлення [name].
CannotContinue=Інсталяція не може бути продовжена. Будь-ласка, натисніть «Скасувати» для виходу.

; *** "Installing" wizard page
WizardInstalling=Інсталяція...
InstallingLabel=Будь ласка, почекайте, поки [name] встановлюється на Ваш комп’ютер.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Завершення Майстра інсталяції [name]
FinishedLabelNoIcons=Інсталяцію [name] на Ваш комп'ютер завершено.
FinishedLabel=Інсталяцію [name] на Ваш комп'ютер завершено. Встановлені програми можна запустити з допомогою створених ярликів.
ClickFinish=Натисніть «Готово» для виходу з інсталяції.
FinishedRestartLabel=Для завершення встановлення [name] необхідно перезавантажити Ваш комп'ютер. Зробити це зараз?
FinishedRestartMessage=Для завершення встановлення [name] необхідно перезавантажити Ваш комп'ютер.%n%nЗробити це зараз?
ShowReadmeCheck=Так, я хочу продивитися файл README
YesRadio=&Так, перезавантажити комп'ютер зараз
NoRadio=&Ні, я перезавантажу комп'ютер пізніше
; used for example as 'Run MyProg.exe'
RunEntryExec=Запуск %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Перегляд %1

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=Потрібен наступний диск
SelectDiskLabel2=Будь ласка, вставте диск %1 і натисніть Далі.%n%nЯкщо потрібні файли знаходяться в іншому, ніж вказано нижче, каталозі, вкажіть правильний шлях або натисніть «Огляд».
PathLabel=&Шлях:
FileNotInDir2=Файл "%1" не може знаходитись в "%2". Будь ласка, вставте правильний диск або вкажіть інший каталог.
SelectDirectoryLabel=Будь ласка, вкажіть шлях до наступного диску.

; *** Installation phase messages
SetupAborted=Інсталяцію не завершено.%n%nБудь ласка, вирішіть проблему і запустіть програму інсталяції знову.
EntryAbortRetryIgnore=Натисніть «Повтор» для повторної спроби, «Ігнорувати» для продовження або «Перервати» для виходу з програми інсталяції.

; *** Installation status messages
StatusCreateDirs=Створення каталогів...
StatusExtractFiles=Розпакування файлів...
StatusCreateIcons=Створення програмних ярликів...
StatusCreateIniEntries=Створення INI-файлів...
StatusCreateRegistryEntries=Створення записів у реєстрі...
StatusRegisterFiles=Реєстрація файлів...
StatusSavingUninstall=Збереження деінсталяційної інформації...
StatusRunProgram=Закінчення інсталяції...
StatusRollback=Скасування змін...

; *** Misc. errors
ErrorInternal2=Внутрішня помилка: %1
ErrorFunctionFailedNoCode=%1 помилка
ErrorFunctionFailed=%1 помилка;; код %2
ErrorFunctionFailedWithMessage=%1 помилка;; код %2.%n%3
ErrorExecutingProgram=Не можу виконати файл:%n%1

; *** Registry errors
ErrorRegOpenKey=Помилка відкриття реєстру:%n%1%2
ErrorRegCreateKey=Помилка створення розділу реєстру:%n%1%2
ErrorRegWriteKey=Помилка запису до реєстру:%n%1%2

; *** INI errors
ErrorIniEntry=Помилка створення INI файлу "%1".

; *** File copying errors
FileAbortRetryIgnore=Натисніть «Повтор» для ще однієї спроби, «Ігнорувати» для пропуску цього файлу (не рекомендується) або «Перервати» для виходу з програми інсталяції.
FileAbortRetryIgnore2=Натисніть «Повтор» для ще однієї спроби, «Ігнорувати» для продовження (не рекомендується) або «Перервати» для виходу з програми інсталяції.
SourceIsCorrupted=Вихідний файл зіпсований
SourceDoesntExist=Вихідний файл "%1" не існує
ExistingFileReadOnly=Існуючий файл має атрибут "тільки для читання".%n%nНатисніть «Повтор» для зняття цього атрибута і повторної спроби, «Ігнорувати» для пропуску цього файла або «Перервати» для виходу з програми інсталяції.
ErrorReadingExistingDest=Помилка при спробі прочитати існуючий файл:
FileExists=Файл вже існує.%n%nПереписати його?
ExistingFileNewer=Існуючий файл новіший за той, що намагається встановити програма інсталяції. Рекомендується зберегти існуючий файл.%n%nЗберегти існуючий файл?
ErrorChangingAttr=Помилка при спробі змінити атрибути існуючого файлу:
ErrorCreatingTemp=Помилка при створенні файлу в цільовому каталозі:
ErrorReadingSource=Помилка при читанні вихідного файлу:
ErrorCopying=Помилка копіювання файлу:
ErrorReplacingExistingFile=Помилка при заміні існуючого файлу:
ErrorRestartReplace=RestartReplace помилка:
ErrorRenamingTemp=Помилка при перейменуванні файлу в цільовому каталозі:
ErrorRegisterServer=Не можу зареєструвати DLL/OCX: %1
ErrorRegisterServerMissingExport=DllRegisterServer експорт не знайдено
ErrorRegisterTypeLib=Не можу зареєструвати тип бібліотеки: %1

; *** Post-installation errors
ErrorOpeningReadme=Помилка відкриття README файлу.
ErrorRestartingComputer=Програма інсталяції не може перезавантажити комп'ютер. Будь ласка, зробіть це вручну.

; *** Uninstaller messages
UninstallNotFound=Файл "%1" не існує. Неможливо деінсталювати.
UninstallOpenError=Файл "%1" не може бути відкритий. Деінсталяція неможлива
UninstallUnsupportedVer=Деінсталятор даної версії не може розпізнати формат деінсталяційного log-файла "%1". Деінсталяція неможлива
UninstallUnknownEntry=Невідомий рядок (%1) знайдено в деінсталяційному log-файлі
ConfirmUninstall=Ви впевнені, що бажаєте повністю видалити %1 і всі компоненти?
UninstallOnlyOnWin64=Дану програму можна деінсталювати тільки в середовищі 64-розрядної Windows
OnlyAdminCanUninstall=Ця програма може бути деінстальована тільки користувачем з адміністративними привілеями.
UninstallStatusLabel=Будь ласка, зачекайте, поки %1 видаляється з вашого комп'ютера.
UninstalledAll=%1 була успішно видалена з вашої машини.
UninstalledMost=Деінсталяцію %1 завершено.%n%nДеякі елементи не вдалося видалити. Спробуйте зробити це вручну.
UninstalledAndNeedsRestart=Для завершення деінсталяції %1 необхідно перезавантажити машину.%n%nПерезавантажити зараз?
UninstallDataCorrupted=Файл "%1" пошкоджений. Неможливо деінсталювати

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=Видалити файл загального користування?
ConfirmDeleteSharedFile2=Система показує, що наступний файл загального користування більше не використовується жодною програмою. Видалити цей файл?%n%n%1%n%nЯкщо деякі програми ще користуються цим файлом, вони можуть працювати неправильно. Якщо ви не впевнені, виберіть «Ні» (це не вплине на нормальну роботу вашої системи).
SharedFileNameLabel=Назва файлу:
SharedFileLocationLabel=Розташування:
WizardUninstalling=Стан деінсталяції
StatusUninstalling=Деінсталяція %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]

NameAndVersion=%1, версія %2
AdditionalIcons=Додаткові ярлики:
CreateDesktopIcon=Створити ярлик на &Робочому столі
CreateQuickLaunchIcon=Створити значок на &Панелі швидкого доступу
ProgramOnTheWeb=Сайт %1 в Інтернеті
UninstallProgram=Деінсталювати %1
LaunchProgram=Запустити %1
AssocFileExtension=Асоціювати %1 з файлами, які мають розширення %2
AssocingFileExtension=Встановлення асоціацій %1 з файлами %2
 