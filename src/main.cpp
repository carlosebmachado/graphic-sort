#include <iostream>
#include <chrono>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define QTD_PONTOS 100
#define QUADRO 900
#define BORDA 40
#define ENTRE_CIRCULOS QUADRO / QTD_PONTOS
#define LARGURA_JANELA (BORDA*3 + QUADRO*2)
#define ALTURA_JANELA (BORDA*3 + QUADRO)
#define SEED_G 50
#define REST 5

#if QTD_PONTOS > QUADRO
#  error A QTD_PONTOS deve ser menor ou igual ao QUADRO
#endif

class Data
{
public:
	float x, y, pad;

	Data()
	{
		x = 0;
		y = 0;
		pad = 0;
	}

	Data(float x, float y)
	{
		this->x = x;
		this->y = y;
		pad = 0;
	}

	void setY(int y)
	{
		this->y = y;
	}

	void render()
	{
		if (ENTRE_CIRCULOS < 2)
		{
			al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
		}
		else
		{
			al_draw_circle(x, y, ENTRE_CIRCULOS / 2, al_map_rgb(255, 255, 255), 1);
		}
	}
};

class Box
{
public:
	float x, y;
	float largura, altura;
	Data dado_grafico[QTD_PONTOS];

	Box()
	{
		x = 0;
		y = 0;
		largura = 0;
		altura = 0;
	}

	Box(float x, float y, float largura, float altura)
	{
		this->x = x;
		this->y = y;
		this->largura = largura + x;
		this->altura = altura + y;
	}

	void render()
	{
		al_draw_rectangle(x, y, largura, altura, al_map_rgb(255, 255, 255), NULL);
		for (int i = 0; i < QTD_PONTOS; i++)
		{
			dado_grafico[i].render();
		}
	}
};

Box bubbleBox;
Box quickBox;
ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_FONT* fonte[2];
ALLEGRO_EVENT_QUEUE* eventQueue = NULL;
bool running = true;
double velocidade = 0;

void render()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonte[0], al_map_rgb(255, 255, 255), 34, 17, NULL, "Bubblesort");
	al_draw_text(fonte[0], al_map_rgb(255, 255, 255), BORDA * 2 + QUADRO, 17, NULL, "Quicksort");
	al_draw_text(fonte[1], al_map_rgb(255, 255, 255), BORDA * 2 + QUADRO, 49, NULL, "* Ajuste a velocidade no fonte");

	bubbleBox.render();
	quickBox.render();
	al_rest(velocidade);

	al_flip_display();
}

void update()
{
	while (!al_is_event_queue_empty(eventQueue))
	{
		ALLEGRO_EVENT aevent;
		al_wait_for_event(eventQueue, &aevent);

		if (aevent.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (aevent.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				running = false;
			}
		}
		else if (aevent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			running = false;
		}
	}
}

void initGraphics()
{
	al_init();
	janela = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_set_window_title(janela, "Graphic Sort");
	eventQueue = al_create_event_queue();
	fonte[0] = al_load_font("fonte.ttf", 28, NULL);
	fonte[1] = al_load_font("fonte.ttf", 16, NULL);

	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(janela));
}

void disposeGraphics()
{
	al_destroy_display(janela);
	al_destroy_font(fonte[0]);
	al_destroy_font(fonte[1]);
}

void initData(int vetor_bubble[], int vetor_quick[])
{
	srand(SEED_G);
	bubbleBox = Box(BORDA, BORDA * 2, QUADRO, QUADRO);
	quickBox = Box(QUADRO + BORDA * 2, BORDA * 2, QUADRO, QUADRO);

	for (int i = 0; i < QTD_PONTOS; i++)
	{
		int numero_gerado = rand() % QTD_PONTOS;
		vetor_bubble[i] = numero_gerado;
		vetor_quick[i] = numero_gerado;
		bubbleBox.dado_grafico[i] = Data(BORDA + i * ENTRE_CIRCULOS, BORDA * 2 + numero_gerado * ENTRE_CIRCULOS);
		quickBox.dado_grafico[i] = Data(QUADRO + BORDA * 2 + i * ENTRE_CIRCULOS, BORDA * 2 + numero_gerado * ENTRE_CIRCULOS);
	}
}

void bubblesort(int vetor[], int n)
{
	int i, j, aux;

	for (i = n - 1; i > 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (vetor[j] > vetor[j + 1])
			{
				bubbleBox.dado_grafico[j].setY((QUADRO + BORDA * 2) - (ENTRE_CIRCULOS * vetor[j + 1]));
				bubbleBox.dado_grafico[j + 1].setY((QUADRO + BORDA * 2) - (ENTRE_CIRCULOS * vetor[j]));
				render();

				aux = vetor[j];
				vetor[j] = vetor[j + 1];
				vetor[j + 1] = aux;
			}
		}
	}
}

void quicksort(int vetor[], int inicio, int fim)
{
	int pivo, aux, i, j, meio;
	i = inicio;
	j = fim;
	meio = (int)((i + j) / 2);
	pivo = vetor[meio];

	do
	{
		while (vetor[i] < pivo) i++;
		while (vetor[j] > pivo) j--;

		if (i <= j)
		{
			quickBox.dado_grafico[i].setY((QUADRO + BORDA * 2) - (ENTRE_CIRCULOS * vetor[j]));
			quickBox.dado_grafico[j].setY((QUADRO + BORDA * 2) - (ENTRE_CIRCULOS * vetor[i]));
			render();

			aux = vetor[i];
			vetor[i] = vetor[j];
			vetor[j] = aux;
			i++;
			j--;
		}
	} while (j > i);

	if (inicio < j) quicksort(vetor, inicio, j);
	if (i < fim) quicksort(vetor, i, fim);
}

int main()
{
	initGraphics();

	int v_bs[QTD_PONTOS];
	int v_qs[QTD_PONTOS];

	velocidade = 0.017;
	initData(v_bs, v_qs);
	bubblesort(v_bs, QTD_PONTOS);

	velocidade = 0.017;
	quicksort(v_qs, 0, QTD_PONTOS - 1);

	while (running)
	{
		update();
		render();
	}

	disposeGraphics();

	return EXIT_SUCCESS;
}
