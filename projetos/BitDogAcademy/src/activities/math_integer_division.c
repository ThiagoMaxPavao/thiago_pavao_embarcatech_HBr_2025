#include "math_integer_division.h"

#define SELECT_DIVIDEND 0
#define SELECT_DIVISOR 1

extern ssd1306_t disp;

// Variaveis da divisao inteira
static int dividend = 7;
static int divisor = 3;
static int quotient;
static int rest;

// Controle do estado do joystick
// extended = inclinado perto do máximo possível
static bool joystick_extended = false;
static bool last_joystick_extended = false;

// Controle do que está sendo editado pelo usuário: Dividendo ou o divisor
static int selected = SELECT_DIVIDEND;

// Controle se deve desenhar a linha abaixo do valor selecionado
// Alternado pelo timer de repeticao para fazer a linha piscar
static bool draw_selected = true;

// Modo de agrupamento -> *Quociente* grupos de *divisor* LEDs ou *divisor* grupos de *quociente* LEDs
static bool grouping_mode = false;

// Indica se é necessário redesenhar o display e a matriz de LEDs
static bool redraw = true;

// Indica se a atividade terminou -> Retornado pela função de loop
static bool leave_activity = false;

static struct repeating_timer switch_draw_selected_and_redraw_timer;

// Timer de repeticao para alternar o desenho da linha abaixo do valor selecionado
bool switch_draw_selected_and_redraw_callback(struct repeating_timer *t) {
    draw_selected = !draw_selected;
    redraw = true;
    return true;
}

// Desenha uma divisao no formato grafico no display OLED
// Desenha uma linha em baixo do valor do dividendo e/ou do divisor, dependendo dos parametros passados
void draw_division_ssd1306(ssd1306_t *disp, int dividend, int divisor, int quotient, int rest, bool dividend_selected, bool divisor_selected) {
    char buffer[3]; // Buffer para a strings formatadas

    ssd1306_clear(disp);

    // Dividendo
    sprintf(buffer, "%2d", dividend);
    ssd1306_draw_string_by_center(disp, disp->width/3, 15, 2, buffer);
    if(dividend_selected) {
        ssd1306_draw_line(disp, disp->width/3-13, 23, disp->width/3+10, 23);
    }

    // Divisor
    sprintf(buffer, "%2d", divisor);
    ssd1306_draw_string_by_center(disp, 2*disp->width/3, 15, 2, buffer);
    if(divisor_selected) {
        ssd1306_draw_line(disp, 2*disp->width/3-13, 23, 2*disp->width/3+10, 23);
    }

    // Quociente
    sprintf(buffer, "%2d", quotient);
    ssd1306_draw_string_by_center(disp, 2*disp->width/3, 45, 2, buffer);

    // Resto
    sprintf(buffer, "%2d", rest);
    ssd1306_draw_string_by_center(disp, disp->width/3, 45, 2, buffer);

    // L da divisão - linha vertical
    ssd1306_draw_line(disp, disp->width/2+1, 5, disp->width/2+1, 27);
    ssd1306_draw_line(disp, disp->width/2+2, 5, disp->width/2+2, 27);
    
    // Linha horizontal
    ssd1306_draw_line(disp, disp->width/2+1, 26, 110, 26);
    ssd1306_draw_line(disp, disp->width/2+2, 27, 110, 27);

    ssd1306_show(disp);
}

// Desenha o agrupamento de LEDs na matriz de LEDs 5x5
void draw_division_np(int dividend, int divisor, int quotient, int rest) {
    int count = 0;

    np_clear();

    uint8_t group_colors[] = {
        255, 0, 0, // Vermelho
        0, 255, 0, // Verde
        0, 0, 255, // Azul
        255, 255, 0, // Amarelo
        255, 0, 255, // Magenta
        0, 255, 255, // Ciano
        255, 127, 0, // Laranja
    };

    int n_colors = sizeof(group_colors) / 3;

    for(int index = 24; index >= 0; index--) {
        if(count >= dividend) break; // end

        if(count < dividend - rest) {
            int color_step = count / quotient;
            int color_i = color_step % n_colors;
            np_set_led_rgb_index(index, group_colors[3*color_i],
                                        group_colors[3*color_i+1],
                                        group_colors[3*color_i+2]);
        } else {
            np_set_led_rgb_index(index, 255, 255, 255);
        }

        count++;
    }
    
    np_write();
}

// Função de callback para o botão A
// Botão sem função na atividade
void math_integer_division_button_A_callback() {
    error_sound();
}

// Função de callback para o botão B
// Botão para sair da atividade
void math_integer_division_button_B_callback() {
    leave_activity = true;
    success_sound();
}

// Função de callback para o joystick
// Alterna entre os modos de agrupamento de LEDs
void math_integer_division_button_joystick_callback() {
    grouping_mode = !grouping_mode;
    redraw = true;
}

void math_integer_division_activity_setup() {
    dividend = 7;
    divisor = 3;
    quotient = dividend / divisor;
    rest = dividend % divisor;

    leave_activity = false;
    grouping_mode = false;
    redraw = true;

    draw_division_ssd1306(&disp, dividend, divisor, quotient, rest, false, false);
    draw_division_np(dividend, divisor, quotient, rest);

    set_button_A_callback(math_integer_division_button_A_callback);
    set_button_B_callback(math_integer_division_button_B_callback);
    set_button_joystick_callback(math_integer_division_button_joystick_callback);

    add_repeating_timer_ms(500, switch_draw_selected_and_redraw_callback, NULL, &switch_draw_selected_and_redraw_timer);
}

bool math_integer_division_activity_loop() {
    float r, angle;

    joystick_get_RA(&r, &angle);

    if(r > 0.9) joystick_extended = true;
    else if(r < 0.8) joystick_extended = false;

    if(joystick_extended && !last_joystick_extended) {

        if(angle_difference(angle, 0) < M_PI/4) {
            if(selected == SELECT_DIVIDEND) {
                selected = SELECT_DIVISOR;
                draw_selected = true;
                success_sound();
            } else {
                error_sound();
            }
        } 
        else if(angle_difference(angle, M_PI) < M_PI_4) {
            if(selected == SELECT_DIVISOR) {
                selected = SELECT_DIVIDEND;
                draw_selected = true;
                success_sound();
            } else {
                error_sound();
            }
        } 
        else if(angle_difference(angle, M_PI_2) < M_PI_4) {
            if(selected) {
                divisor++;
            } else {
                dividend++;
            }
            success_sound();
        } 
        else {
            if(selected) {
                divisor--;
            } else {
                dividend--;
            }
            success_sound();
        }

        divisor = clamp(divisor, 1, 25);
        dividend = clamp(dividend, 0, 25);

        quotient = dividend / divisor;
        rest = dividend % divisor;

        redraw = true;
    }

    if(redraw) {
        draw_division_ssd1306(&disp, dividend, divisor, quotient, rest, draw_selected && !selected, draw_selected && selected);
        draw_division_np(dividend, grouping_mode ? divisor : quotient, grouping_mode ? quotient : divisor, rest);
    }
    
    last_joystick_extended = joystick_extended;

    sleep_ms(20);

    return leave_activity;
}

bool math_integer_division_draw_tutorial_page(int page_number) {
    ssd1306_clear(&disp);

    switch(page_number) {
        case 0:
        ssd1306_draw_string_with_font(&disp, 8, 16, 2, acme_font, "Divisao");
        ssd1306_draw_string_with_font(&disp, 8, 36, 2, acme_font, "Inteira");
        break;

        case 1:                                                 // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Pressione B para ");
        ssd1306_draw_string(&disp, 2, 12, 1, "passar para a proxima");
        ssd1306_draw_string(&disp, 2, 22, 1, "pagina.");
        ssd1306_draw_string(&disp, 2, 32, 1, "Pressione o joystick");
        ssd1306_draw_string(&disp, 2, 42, 1, "para pular direto ");
        ssd1306_draw_string(&disp, 2, 52, 1, "para a atividade.");
        break;

        case 2:                                                 // |
        ssd1306_draw_string(&disp, 2, 12, 1, "A divisao inteira eh");
        ssd1306_draw_string(&disp, 2, 22, 1, "uma forma de dividir");
        ssd1306_draw_string(&disp, 2, 32, 1, "sem deixar partes");
        ssd1306_draw_string(&disp, 2, 42, 1, "quebradas.");
        break;

        case 3:                                                 // |
        ssd1306_draw_string(&disp, 2, 12, 1,  "Imagine que voce tem");
        ssd1306_draw_string(&disp, 2, 22, 1, "7 balas e quer");
        ssd1306_draw_string(&disp, 2, 32, 1, "dividir igualmente");
        ssd1306_draw_string(&disp, 2, 42, 1, "entre 3 amigos.");
        break;

        case 4:                                                 // |
        ssd1306_draw_string(&disp, 2, 12, 1,  "Se voce der 2 balas");
        ssd1306_draw_string(&disp, 2, 22, 1, "para cada um, tera");
        ssd1306_draw_string(&disp, 2, 32, 1, "usado 6 balas e ");
        ssd1306_draw_string(&disp, 2, 42, 1, "sobrara 1 bala.");
        break;

        case 5:                                                 // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Estes numeros sao");
        ssd1306_draw_string(&disp, 2, 12, 1, "chamados de:");
        ssd1306_draw_string(&disp, 2, 22, 1, "- Dividendo: 7");
        ssd1306_draw_string(&disp, 2, 32, 1, "- Divisor: 3");
        ssd1306_draw_string(&disp, 2, 42, 1, "- Quociente: 2");
        ssd1306_draw_string(&disp, 2, 52, 1, "- Resto: 1");
        break;

        case 6:                                                 // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Outra forma de pensar");
        ssd1306_draw_string(&disp, 2, 12, 1, "na divisao eh:");
        ssd1306_draw_string(&disp, 2, 22, 1, "Quero dividir 7 balas");
        ssd1306_draw_string(&disp, 2, 32, 1, "dando 3 balas para");
        ssd1306_draw_string(&disp, 2, 42, 1, "cada um, com quantos");
        ssd1306_draw_string(&disp, 2, 52, 1, "amigos posso dividir?");
        break;

        case 7:                                                 // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "O resultado sera 2");
        ssd1306_draw_string(&disp, 2, 12, 1, "novamente, mas desta");
        ssd1306_draw_string(&disp, 2, 22, 1, "vez representando o");
        ssd1306_draw_string(&disp, 2, 32, 1, "numero de amigos.");
        ssd1306_draw_string(&disp, 2, 42, 1, "O resto tambem eh");
        ssd1306_draw_string(&disp, 2, 52, 1, "igual. Sobra uma bala");
        break;

        case 8:                                                 // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "A divisao pode ser");
        ssd1306_draw_string(&disp, 2, 12, 1, "representada de forma");
        ssd1306_draw_string(&disp, 2, 22, 1, "grafica, como");
        ssd1306_draw_string(&disp, 2, 32, 1, "mostrado a seguir");
        ssd1306_draw_string(&disp, 2, 42, 1, "para o exemplo das");
        ssd1306_draw_string(&disp, 2, 52, 1, "7 balas.");
        break;

        case 9:
        draw_division_ssd1306(&disp, 7, 3, 2, 1, false, false);
        break;

        case 10:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1,  "Nesta atividade voce");
        ssd1306_draw_string(&disp, 2, 22, 1, "visualizara a divisao");
        ssd1306_draw_string(&disp, 2, 32, 1, "dos LEDs presentes na");
        ssd1306_draw_string(&disp, 2, 42, 1, "matriz 5x5.");
        break;

        case 11:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1,  "Voce controlara o");
        ssd1306_draw_string(&disp, 2, 22, 1, "valor do dividendo e");
        ssd1306_draw_string(&disp, 2, 32, 1, "do divisor, faca isso");
        ssd1306_draw_string(&disp, 2, 42, 1, "utilizando o joystick");
        break;

        case 12:                                                // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Mova o joystick para");
        ssd1306_draw_string(&disp, 2, 12, 1, "a esquerda/direita");
        ssd1306_draw_string(&disp, 2, 22, 1, "para trocar o valor");
        ssd1306_draw_string(&disp, 2, 32, 1, "que esta editando.");
        ssd1306_draw_string(&disp, 2, 42, 1, "(Entre o divisor e");
        ssd1306_draw_string(&disp, 2, 52, 1, " o dividendo)");
        break;

        case 13:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1, "Voce sabera o valor");
        ssd1306_draw_string(&disp, 2, 22, 1, "que esta editando");
        ssd1306_draw_string(&disp, 2, 32, 1, "atentando-se a linha");
        ssd1306_draw_string(&disp, 2, 42, 1, "piscando abaixo dele.");
        break;

        case 14:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1, "Mova o joystick para");
        ssd1306_draw_string(&disp, 2, 22, 1, "cima/baixo para");
        ssd1306_draw_string(&disp, 2, 32, 1, "aumentar ou diminuir");
        ssd1306_draw_string(&disp, 2, 42, 1, "o valor selecionado.");
        break;

        case 15:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1, "Na matriz de LEDs,");
        ssd1306_draw_string(&disp, 2, 22, 1, "voce vera N LEDs");
        ssd1306_draw_string(&disp, 2, 32, 1, "acesos, sendo N o");
        ssd1306_draw_string(&disp, 2, 42, 1, "dividendo escolhido.");
        break;

        case 16:                                                // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Eles estarao");
        ssd1306_draw_string(&disp, 2, 12, 1, "agrupados pela cor,");
        ssd1306_draw_string(&disp, 2, 22, 1, "conforme o divisor ou");
        ssd1306_draw_string(&disp, 2, 32, 1, "o quociente. Aperte");
        ssd1306_draw_string(&disp, 2, 42, 1, "o joystick para ver ");
        ssd1306_draw_string(&disp, 2, 52, 1, "ambos os agrupamentos");
        break;

        case 17:                                                // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "O resto da divisao eh");
        ssd1306_draw_string(&disp, 2, 12, 1, "representado pelos ");
        ssd1306_draw_string(&disp, 2, 22, 1, "LEDs de cor branca.");
        ssd1306_draw_string(&disp, 2, 32, 1, "Quando nao houver");
        ssd1306_draw_string(&disp, 2, 42, 1, "nenhum, diz-se que a");
        ssd1306_draw_string(&disp, 2, 52, 1, "divisao foi exata.");
        break;
        
        case 18:                                                // |
        ssd1306_draw_string(&disp, 2, 2, 1,  "Teste combinacoes");
        ssd1306_draw_string(&disp, 2, 12, 1, "e atente-se como os");
        ssd1306_draw_string(&disp, 2, 22, 1, "LEDs se comportam.");
        ssd1306_draw_string(&disp, 2, 32, 1, "Nao esqueca-se de");
        ssd1306_draw_string(&disp, 2, 42, 1, "pressionar o joystick");
        break;

        case 19:                                                // |
        ssd1306_draw_string(&disp, 2, 12, 1, "Quando terminar,");
        ssd1306_draw_string(&disp, 2, 22, 1, "Pressione o botao B");
        ssd1306_draw_string(&disp, 2, 32, 1, "para sair da");
        ssd1306_draw_string(&disp, 2, 42, 1, "atividade");
        break;

        default:
        return true;
    }

    ssd1306_show(&disp);
    return false;
}
