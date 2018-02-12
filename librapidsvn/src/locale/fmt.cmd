for %%X in (de fr hu it_IT pt_BR ru es uk zh_CN ja) do (
  %GETTEXT%\msgfmt.exe -o "%%X\rapidsvn.mo" "%%X\rapidsvn.po"
  md ..\..\..\..\dist\locale\%%X
  copy /y %%X\rapidsvn.* ..\..\..\..\dist\locale\%%X
)
