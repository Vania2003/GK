Dla uruchomienia strony z załadowanymi teksturami, bez korzystania z serwerów lokalnych w przeglądarce "Google Chrome", trzeba:
-otworzyć właściwości przeglądarki (np. naciśnięciem prawym klawiszem na ikonkę Chrome na pułpicie)
-w polu Element(Target) na końcu dodać flagi: --disable-web-security --allow-file-access-from-files
-całość powinna wyglądać tak: "C:\Program Files\Google\Chrome\Application\chrome.exe" --disable-web-security --allow-file-access-from-files
UWAGA: uruchamianie w takim trybie wyłącza wszystkie bezpieczeństwa CORS i ochrony przez nieautyryzowanym dostępem do plików.
Nie używać na codzień.