/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:xvales02$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;
const int normalize = 16581375;
float my_timer = 0.0;

/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    /* napr. renderer->base.releaseFunc = studrenRelease; */
    renderer->base.releaseFunc = studrenRelease;
    renderer->base.projectTriangleFunc = studrenProjectTriangle;

    /* inicializace nove pridanych casti */
    renderer->texture = loadBitmap(TEXTURE_FILENAME, &(renderer->frame_w), &(renderer->frame_h));

    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
        free(renderer->texture);
        
        /* fce default rendereru */
        renRelease(ppRenderer);
    }
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         S_Coords *t, double a, double b, double c
                         )
{
    int         minx, miny, maxx, maxy;
    int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int         s1, s2, s3;
    int         x, y, e1, e2, e3;
    double      alpha, beta, gamma, w1, w2, w3, u, v, z;
    S_RGBA      col1, col2, col3, color, texture_color;

    IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

    /* vypocet barev ve vrcholech */
    col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
    col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
    col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

    /* obalka trojuhleniku */
    minx = MIN(x1, MIN(x2, x3));
    maxx = MAX(x1, MAX(x2, x3));
    miny = MIN(y1, MIN(y2, y3));
    maxy = MAX(y1, MAX(y2, y3));

    /* oriznuti podle rozmeru okna */
    miny = MAX(miny, 0);
    maxy = MIN(maxy, pRenderer->frame_h - 1);
    minx = MAX(minx, 0);
    maxx = MIN(maxx, pRenderer->frame_w - 1);

    /* Pineduv alg. rasterizace troj.
       hranova fce je obecna rovnice primky Ax + By + C = 0
       primku prochazejici body (x1, y1) a (x2, y2) urcime jako
       (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

    /* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
    a1 = y1 - y2;
    a2 = y2 - y3;
    a3 = y3 - y1;
    b1 = x2 - x1;
    b2 = x3 - x2;
    b3 = x1 - x3;

    /* koeficient C */
    c1 = x1 * y2 - x2 * y1;
    c2 = x2 * y3 - x3 * y2;
    c3 = x3 * y1 - x1 * y3;

    /* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
    s1 = a1 * x3 + b1 * y3 + c1;
    s2 = a2 * x1 + b2 * y1 + c2;
    s3 = a3 * x2 + b3 * y2 + c3;

    if ( !s1 || !s2 || !s3 )
    {
        return;
    }

    /* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
    if( s1 < 0 )
    {
        a1 *= -1;
        b1 *= -1;
        c1 *= -1;
    }
    if( s2 < 0 )
    {
        a2 *= -1;
        b2 *= -1;
        c2 *= -1;
    }
    if( s3 < 0 )
    {
        a3 *= -1;
        b3 *= -1;
        c3 *= -1;
    }

    /* koeficienty pro barycentricke souradnice */
    alpha = 1.0 / ABS(s2);
    beta = 1.0 / ABS(s3);
    gamma = 1.0 / ABS(s1);

    /* vyplnovani... */
    for( y = miny; y <= maxy; ++y )
    {
        /* inicilizace hranove fce v bode (minx, y) */
        e1 = a1 * minx + b1 * y + c1;
        e2 = a2 * minx + b2 * y + c2;
        e3 = a3 * minx + b3 * y + c3;

        for( x = minx; x <= maxx; ++x )
        {
            if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
            {
                /* interpolace pomoci barycentrickych souradnic
                   e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
                w1 = alpha * e2;
                w2 = beta * e3;
                w3 = gamma * e1;

                /* interpolace z-souradnice */
                z = w1 * v1->z + w2 * v2->z + w3 * v3->z;
                u = (w1 * t[0].x / a + w2 * t[1].x / b + w3 * t[2].x / c) / (w1 / a + w2 / b + w3 / c);
                v = (w1 * t[0].y / a + w2 * t[1].y / b + w3 * t[2].y / c) / (w1 / a + w2 / b + w3 / c);

                texture_color = studrenTextureValue((struct S_StudentRenderer *)pRenderer, u, v);
               
                /* interpolace barvy */
                color.alpha = 255;
                color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red) * texture_color.red / color.alpha;
                color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green) * texture_color.green / color.alpha;
                color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue) * texture_color.blue / color.alpha;

                /* vykresleni bodu */
                if( z < DEPTH(pRenderer, x, y) )
                {
                    PIXEL(pRenderer, x, y) = color;
                    DEPTH(pRenderer, x, y) = z;
                }
            }

            /* hranova fce o pixel vedle */
            e1 += a1;
            e2 += a2;
            e3 += a3;
        }
    }
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
    S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
    S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
    S_Coords    nn;                     /* normala trojuhelniku po transformaci */
    S_Coords	i0_vec, i1_vec, i2_vec, in_vec, i0_vec_next, i1_vec_next, i2_vec_next, in_vec_next;
    int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
    S_Triangle  * triangle;
    int         vertexOffset, normalOffset, /* offset pro vrcholy a normalove vektory trojuhelniku */
    vertexOffset_next, normalOffset_next; /* offset pro vrcholy a normalove vektory trojuhelniku pro nasledujici vrchol */
    int         i0, i1, i2, in, i0_next, i1_next, i2_next, in_next; /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho a nasl. snimku */
    float t;		/* parametr pro interpolaci */
    double a, b, c;

    IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );

    /* z modelu si vytahneme i-ty trojuhelnik */
    triangle = trivecGetPtr(pModel->triangles, i);

    /* ziskame offset pro vrcholy n-teho snimku */
    vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro vrcholy (n+1)-teho snimku */
    vertexOffset_next = (((int) n + 1) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n-teho snimku */
    normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;

    /* ziskame offset pro normaly trojuhelniku (n+1)-teho snimku */
    normalOffset_next = (((int) n + 1) % pModel->frames) * pModel->triangles->size;

    /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    i0 = triangle->v[ 0 ] + vertexOffset;
    i1 = triangle->v[ 1 ] + vertexOffset;
    i2 = triangle->v[ 2 ] + vertexOffset;

    /* indexy vrcholu pro i-ty trojuhelnik (n+1)-teho snimku - pricteni offsetu */
    i0_next = triangle->v[ 0 ] + vertexOffset_next;
    i1_next = triangle->v[ 1 ] + vertexOffset_next;
    i2_next = triangle->v[ 2 ] + vertexOffset_next;

    /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    in = triangle->n + normalOffset;

    /* index normaloveho vektoru pro i-ty trojuhelnik (n+1)-teho snimku - pricteni offsetu */
    in_next = triangle->n + normalOffset_next;

	/* interpolace mezi vrcholy */
	i0_vec = *(cvecGetPtr(pModel->vertices, i0));
	i1_vec = *(cvecGetPtr(pModel->vertices, i1));
	i2_vec = *(cvecGetPtr(pModel->vertices, i2));

	i0_vec_next = *(cvecGetPtr(pModel->vertices, i0_next));
	i1_vec_next = *(cvecGetPtr(pModel->vertices, i1_next));
	i2_vec_next = *(cvecGetPtr(pModel->vertices, i2_next));

	t = n - (int) n;
	i0_vec.x = (i0_vec.x * (1 - t)) + (t * i0_vec_next.x);
	i0_vec.y = (i0_vec.y * (1 - t)) + (t * i0_vec_next.y);
	i0_vec.z = (i0_vec.z * (1 - t)) + (t * i0_vec_next.z);
	i1_vec.x = (i1_vec.x * (1 - t)) + (t * i1_vec_next.x);
	i1_vec.y = (i1_vec.y * (1 - t)) + (t * i1_vec_next.y);
	i1_vec.z = (i1_vec.z * (1 - t)) + (t * i1_vec_next.z);
	i2_vec.x = (i2_vec.x * (1 - t)) + (t * i2_vec_next.x);
	i2_vec.y = (i2_vec.y * (1 - t)) + (t * i2_vec_next.y);
	i2_vec.z = (i2_vec.z * (1 - t)) + (t * i2_vec_next.z);

    /* transformace vrcholu matici model */
    trTransformVertex(&aa, &(i0_vec));
    trTransformVertex(&bb, &(i1_vec));
    trTransformVertex(&cc, &(i2_vec));

    /* promitneme vrcholy trojuhelniku na obrazovku */
    a = trProjectVertex(&u1, &v1, &aa);
    b = trProjectVertex(&u2, &v2, &bb);
    c = trProjectVertex(&u3, &v3, &cc);

	/* interpolace mezi vektory normal */
	i0_vec = *(cvecGetPtr(pModel->normals, i0));
	i1_vec = *(cvecGetPtr(pModel->normals, i1));
	i2_vec = *(cvecGetPtr(pModel->normals, i2));

	i0_vec_next = *(cvecGetPtr(pModel->normals, i0_next));
	i1_vec_next = *(cvecGetPtr(pModel->normals, i1_next));
	i2_vec_next = *(cvecGetPtr(pModel->normals, i2_next));

	i0_vec.x = (i0_vec.x * (1 - t)) + (t * i0_vec_next.x);
	i0_vec.y = (i0_vec.y * (1 - t)) + (t * i0_vec_next.y);
	i0_vec.z = (i0_vec.z * (1 - t)) + (t * i0_vec_next.z);
	i1_vec.x = (i1_vec.x * (1 - t)) + (t * i1_vec_next.x);
	i1_vec.y = (i1_vec.y * (1 - t)) + (t * i1_vec_next.y);
	i1_vec.z = (i1_vec.z * (1 - t)) + (t * i1_vec_next.z);
	i2_vec.x = (i2_vec.x * (1 - t)) + (t * i2_vec_next.x);
	i2_vec.y = (i2_vec.y * (1 - t)) + (t * i2_vec_next.y);
	i2_vec.z = (i2_vec.z * (1 - t)) + (t * i2_vec_next.z);

    /* pro osvetlovaci model transformujeme take normaly ve vrcholech */
    trTransformVector(&naa, &(i0_vec));
    trTransformVector(&nbb, &(i1_vec));
    trTransformVector(&ncc, &(i2_vec));

    /* normalizace normal */
    coordsNormalize(&naa);
    coordsNormalize(&nbb);
    coordsNormalize(&ncc);

    /* interpolace mezi dvema trinormalami */
    in_vec = *(cvecGetPtr(pModel->trinormals, in));
    in_vec_next = *(cvecGetPtr(pModel->trinormals, in_next));

    in_vec.x = (in_vec.x * (1 - t)) + (t * in_vec_next.x);
    in_vec.y = (in_vec.y * (1 - t)) + (t * in_vec_next.y);
    in_vec.z = (in_vec.z * (1 - t)) + (t * in_vec_next.z);

    /* transformace normaly trojuhelniku matici model */
    trTransformVector(&nn, &(in_vec));
    
    /* normalizace normaly */
    coordsNormalize(&nn);

    /* je troj. privraceny ke kamere, tudiz viditelny? */
    if( !renCalcVisibility(pRenderer, &aa, &nn) )
    {
        /* odvracene troj. vubec nekreslime */
        return;
    }

    /* rasterizace trojuhelniku */
    studrenDrawTriangle(pRenderer,
                    &aa, &bb, &cc,
                    &naa, &nbb, &ncc,
                    u1, v1, u2, v2, u3, v3,
                    triangle->t, a, b, c
                    );
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
	double w, h;
	S_RGBA b_1, b_2, b_3, b_4;
	int int_w, int_h;

	w = pRenderer->frame_w * u;
	h = pRenderer->frame_h * v;

	int_w = (w - (int) w < 0.5) ? (int) (w - 1) : (int) w;
	int_h = (h - (int) h < 0.5) ? (int) (h - 1) : (int) h;

	w -= int_w;
	h -= int_h;

	w = (w > 0.5) ? w - 0.5 : w + 0.5;
	h = (h > 0.5) ? h - 0.5 : h + 0.5;

	b_1 = pRenderer->texture[pRenderer->frame_h * int_w + int_h];
	b_2 = pRenderer->texture[pRenderer->frame_h * int_w + int_h + 1];
	b_3 = pRenderer->texture[pRenderer->frame_h * (int_w + 1) + int_h];
	b_4 = pRenderer->texture[pRenderer->frame_h * (int_w + 1) + int_h + 1];

	b_1.red = (1 - h) * b_1.red + b_2.red * h;
	b_1.green = (1 - h) * b_1.green + b_2.green * h;
	b_1.blue = (1 - h) * b_1.blue + b_2.blue * h;
	b_1.alpha = (1 - h) * b_1.alpha + b_2.alpha * h;

	b_3.red = (1 - h) * b_3.red + b_4.red * h;
	b_3.green = (1 - h) * b_3.green + b_4.green * h;
	b_3.blue = (1 - h) * b_3.blue + b_4.blue * h;
	b_3.alpha = (1 - h) * b_3.alpha + b_4.alpha * h;

	b_1.red = (1 - w) * b_1.red + b_3.red * w;
	b_1.green = (1 - w) * b_1.green + b_3.green * w;
	b_1.blue = (1 - w) * b_1.blue + b_3.blue * w;
	b_1.alpha = (1 - w) * b_1.alpha + b_3.alpha * w;

	return makeColor(b_1.red, b_1.green, b_1.blue);

    //unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    //return makeColor( c, 255 - c, 0 );
}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
    /* test existence frame bufferu a modelu */
    IZG_ASSERT(pModel && pRenderer);

    /* nastavit projekcni matici */
    trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

    /* vycistit model matici */
    trLoadIdentity();

    /* nejprve nastavime posuv cele sceny od/ke kamere */
    trTranslate(0.0, 0.0, pRenderer->scene_move_z);

    /* nejprve nastavime posuv cele sceny v rovine XY */
    trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

    /* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
    trRotateX(pRenderer->scene_rot_x);
    trRotateY(pRenderer->scene_rot_y);

    static S_Material  MAT_WHITE_AMBIENT = {1.0, 1.0, 1.0, 1.0};        /* ambientni (rozptylena) slozka materialu */
    static S_Material  MAT_WHITE_DIFFUSE = {1.0, 1.0, 1.0, 1.0};        /* difuzni (matna) slozka materialu */
    static S_Material  MAT_WHITE_SPECULAR = {1.0, 1.0, 1.0, 1.0};       /* spekularni (odleskova) slozka materialu */

    /* nastavime material */
    renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
    renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
    renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

    /* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
    renderModel(pRenderer, pModel, my_timer);
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku
     * a pro interpolaci mezi snimky */
    my_timer = (float) ticks / 100.0;
}

/*****************************************************************************
 *****************************************************************************/
