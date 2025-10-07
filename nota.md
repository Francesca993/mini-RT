# Flusso di azione:

**Dal main:**
-> Alloco memoria per la t_scene
-> Controllo che il file abbuia effettivamente l'estensione .rt (Valuterò dove spostarla in seguito).
-> parte **parse_file** che prende il puntatore al fd e la t_scene.

### Parse_file
-> parse_file inizializza subito la t_scene.
-> Apre il file con Open.
-> Legge la riga con GNL:
    - se è vuota la salta.
    - Altrimenti toglie il '\n' finale e mette '\0', e la pass a **lex_scan_check_and_count**.  

### NB: Assicurarmi di aggiungere al punto giusto credo qui, la funzione che in caso di errore libera correttamente tutta la memoria allocata.

### lex_scan_check_and_count

-> Prende la t_scene e la riga appena letta.
-> Prende la rioga e fa il primo controllo sul primo pezzo di token, ovvero l'ID, quello che dovrebbe identificare iul parametro con **check_first_token**, che fa due cose importanti:
    - Salta eventuali spazi iniziali e prende il primi caratteri.
    - prende i primi max 10 caratteri fino al prossimio spazio, e verifica che siano identificatori validi per i requisiti con **is_valid_identifier**.
        - Se lo sono: restituisce l'ID, e nel rest_out il puntatore al continuo della line.
        - Se NON lo sono NULL e quindi errore: IOdentificatorenon valido.
-> Se **check_first_token** va a buon fine a questo punto, sto implementando le funzioni che a seconda dell'id riconosciuto allocheranno il resto della roga mentre faranno gia anche i check che i valori siano corretti e nei range per ogni elemento, che non ci siano piu elementi la dove non dovessero essercene.

se con ft_strcmp dovesse trovare una "A":
### parse_ambient_line;

**int	parse_double(const char **line, double *out)**  -> Helper che estrae il double dalla stringa
**parse_rgb** -> Helper che estrae l'RGB dalla stringa