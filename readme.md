Allora mi sono basata sul subject e ho creato per ogni elemento una struttura, e su ogni struttura ho messo ogni parametro che dovrebbe avere secondo il subject.
All'inizio del mio main c'è una funzione di check sul file, che sia .rt. vorrei poi implementare una funzione unica che comprenderà anche quella, a cui si passerà l'fd e il file e lo controllerà prima di aprirlo. ma per il momento l'ho messa li. 
ora sto stilando una lista completa di tutti i check da fare mentre carico il file e di tutto cio che devo controllare secondo il subject.

**Fatto e da fare:**
-> ho fatto una funzione che legge il file con get_next_line, ne prende il primo frammento e a seconda dell id partira una funzione che farà il check su i parametri e se corretti creerà la struttura dati. Allo stesso tempo tiene il conto nella t_scene delle presenze. 

Estensione .rt già verificata per il momento nel main ✅

Righe vuote e spazi: puoi ignorare righe vuote o solo spazi/tab. Gli elementi sono separati da uno o più spazi/tabs. -> fare una funzione che ignora righe e spazi nelle utils


Identificatori validi: solo questi token a inizio riga sono permessi nel mandatory: A, C, L, sp, pl, cy. Qualsiasi altro identificatore → errore di parsing.

✅ Presenze obbligatorie e unicità:
A una sola volta.
C una sola volta.
L una sola volta nel mandatory (nel bonus potranno essere più luci).

✅ Almeno un oggetto (sp/pl/cy) deve essere presente per avere qualcosa da renderizzare (se non c’è nulla → errore logico di scena vuota).

Formato dei gruppi con virgole: 
- coordinate/orientazioni/colori sono sempre nel formato x,y,z e R,G,B 
- senza spazi dentro; 
- devono avere esattamente 3 componenti. 
- Se ci sono spazi, componenti in più/meno, o virgole doppie → ERRORE.

Numero di parametri: 
ogni riga deve avere esattamente i parametri previsti, né uno in più né uno in meno. 
In eccesso/mancanza → errore.

Tipi numerici: 
coordinate e dimensioni -> sono floating (accetta segno e punto decimale). 
Colori -> sono interi base 10.
Valori NaN/Inf -> rifiutati.



Vincoli di range e coerenza (per tipo di valore)
Colori R,G,B: interi 0–255. Qualsiasi valore fuori range → errore.

Ratio/Intensità:
A.ratio: 0.0–1.0.
L.brightness (intensità): 0.0–1.0.
FOV (camera): in gradi, 0 < FOV < 180. 0 o 180 o più → errore.

Orientazioni/vettori direzione (C dir, pl normal, cy axis):
Ogni componente deve stare in [-1, 1].
Il vettore non può essere (0,0,0).
Deve essere normalizzato (norma ≈ 1 entro una tolleranza). Se non lo è, nel mandatory si rifiuta la riga (niente auto-normalizzazione silenziosa).

Dimensioni positive:
sp diameter > 0.
cy diameter > 0 e height > 0.
Ordine dei valori per ogni elemento (mandatory)
L’ordine deve essere esattamente questo. Se l’ordine non è rispettato o manca/un extra parametro → errore di parsing.

Ambient (A)
A ratio R,G,B
ratio ∈ [0,1]
R,G,B interi 0–255

Camera (C)
C x,y,z nx,ny,nz FOV
x,y,z posizione
nx,ny,nz direzione (norm. in [-1,1], ≠(0,0,0))
FOV in gradi, 0<FOV<180

Light (L)
L x,y,z brightness R,G,B
x,y,z posizione
brightness ∈ [0,1]
R,G,B interi 0–255

Sphere (sp)
sp x,y,z diameter R,G,B
x,y,z centro
diameter > 0
R,G,B interi 0–255

Plane (pl)
pl x,y,z nx,ny,nz R,G,B
x,y,z un punto sul piano
nx,ny,nz normale (norm. in [-1,1], ≠(0,0,0))
R,G,B interi 0–255

Cylinder (cy)
cy x,y,z nx,ny,nz diameter height R,G,B
x,y,z punto base (o centro, dipende dalla tua convenzione, ma deve essere consistente)
nx,ny,nz asse (norm. in [-1,1], ≠(0,0,0))
diameter > 0
height > 0
R,G,B interi 0–255


Cosa succede se…
Ordine sbagliato: es. C FOV x,y,z nx,ny,nz → errore (riga rifiutata).

Parametro mancante o extra: es. sp x,y,z diameter (manca colore) o A ratio R,G,B extra → errore.

Orientazione non normalizzata / fuori [-1,1] / zero vector: errore (non normalizzare tacitamente nel mandatory).

Valori fuori range (colori, ratio, FOV, dimensioni non positive): errore.

Duplicati A/C/L: se trovi una seconda istanza → errore. 

# SPIEGAZIONE T_UNION

- come definire le struct,
- come costruire gli oggetti nel parsing,
- come userarli nel rendering.

🧩 1. Strutture dati principali
Queste vanno nel tuo miniRT_struct.h o in un file tipo scene.h.

 ```
typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}	t_object_type;

/* ───────────── Oggetti geometrici ───────────── */

typedef struct s_sphere
{
	t_vec3	center;
	double	diameter;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_color	color;
}	t_cylinder;

/* ───────────── Contenitore generico ───────────── */

typedef struct s_object
{
	t_object_type	type;   // serve per capire cosa contiene il union
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
	}	as;                  // "as" = as.sphere / as.plane / as.cylinder
}	t_object;

/* ───────────── Scena completa ───────────── */

typedef struct s_scene
{
	t_camera	camera;
	t_light		light;
	t_ambient	ambient;

	t_object	*objects;     // array dinamico di oggetti
	size_t		object_count; // quanti oggetti totali
	size_t		object_cap;   // capacità allocata
}	t_scene;

 ```