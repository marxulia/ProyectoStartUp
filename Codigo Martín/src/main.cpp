#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#define TIMESTEP 1.0f/60.0f     //TIEMPO DE REFRESCO
#define VELITER 10              //NUMERO DE ITERACION POR TICK PARA CALCULAR LA VELOCIDAD
#define POSITER 10              //NUMERO DE ITERACIONES POR TICK PARA CALCULAR LA POSICION

#define PPM 64.0f               //PIXELS POR METRO
#define MPP (1.0f/PPM)          //METROS POR PIXEL
#include <iostream>
using namespace std;

struct Player{
    Player (sf::Vector2f size){
        sizePlayer = size;
    }

    sf::Vector2f sizePlayer;
    b2Body* p_bodyPlayer;
};

///Estructura con los datos para representar una caja
struct Caja{
    Caja() {
        m_Size = sf::Vector2f(10.0f, 10.0f);
    }
    Caja(sf::Vector2f size){
        m_Size = size;
    }

    sf::Vector2f m_Size;
	b2Body *m_pBody;  /// Necesario para obtener los datos generados por Box2D para esta 'pieza'
};


int main()
{
    /// Vector con las cajas a dibujar
    std::vector<Caja> m_vCajas;

    sf::Texture tPlayer;


    if(!tPlayer.loadFromFile("assets/player_walking.png")){
        // Si hay un error salimos
        cout << " Error al cargar >assets/player_walking.png";
		return EXIT_FAILURE;
    }
    sf::Sprite sPlayer;

    sPlayer.setTexture(tPlayer);
    sPlayer.setTextureRect(sf::IntRect(0, 0, 28, 49));


    /* Definir mundo y su gravedad
    ------------------------------------------------------------------------*/
    /// Definir Mundo Box2D (El parametro es la gravedad)
    b2Vec2 Gravity(0.0f, 9.8f);
    b2World world(Gravity);

    /* Definir Suelo
    ------------------------------------------------------------------------*/
	b2BodyDef sueloBodyDef;
	sueloBodyDef.position.Set(400.0f*MPP, 550.0f*MPP);

	/** Creamos u  puntero de tipo b2Body */
	b2Body* sueloBody;

	/** sueloBody ahora apunta a un body creado en el mundo (Ahora el body se encuentra dentro del mumdo) */
	sueloBody = world.CreateBody(&sueloBodyDef);

    /** Creamos un poligono (cuadrado por los paraamtros pasados) */
	b2PolygonShape sueloBox;
	sueloBox.SetAsBox(800.0f/2*MPP, 100.0f/2*MPP);

    /** Le asignamos el shape al body anterioremente creado y le asigamos la densidad 0.0f */
	sueloBody->CreateFixture(&sueloBox, 0.0f);


	/* Definir Player
    ------------------------------------------------------------------------*/
    Player myPlayer = Player(sf::Vector2f(10.0f, 50.0f)); // Esta en pixeles

    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(300.0f*MPP, 300.0f*MPP);

	/** Creamos u  puntero de tipo b2Body */
	b2Body* playerBody;

	/** playerBody ahora apunta a un body creado en el mundo (Ahora el body player se encuentra dentro del mumdo) */
	myPlayer.p_bodyPlayer = world.CreateBody(&playerBodyDef);

    /** Creamos un poligono (cuadrado por los paraamtros pasados) */
	b2PolygonShape playerBox;
	playerBox.SetAsBox(20.0f/2*MPP, 50.0f/2*MPP);

    /** Le asignamos el shape al body anterioremente creado y le asigamos la densidad 0.0f */
	myPlayer.p_bodyPlayer->CreateFixture(&playerBox, 0.0f);

	/*  Ventana
	------------------------------------------------------------------------*/
    sf::RenderWindow window(sf::VideoMode(800, 600), "My First Game e.e");
    window.setFramerateLimit(60); //Hacer que los tiempos de refresco casen con los tiempos de Box2D

    /* Bucle del juego donde estaran los eventos
    ------------------------------------------------------------------------*/
    while (window.isOpen())
    {
        /// Bucle de Eventos de ventana
        sf::Event event;
        while (window.pollEvent(event))
        {
            /// Cerrar la ventana cuando se pulsa en el boton de cerrar ventana
            if (event.type == sf::Event::Closed)
                window.close();



            /// Crear una caja donde el usuario hace click izquierdo
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f posRaton = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    /// Creamos un nuevo objeto caja
                    Caja nuevaCaja = Caja(sf::Vector2f(50, 20));  // dimesiones de la caja

                    /// Creamos un poligono, fixture y un body
                    b2PolygonShape polyShape;
                    b2FixtureDef fixtureDef;
                    b2BodyDef bodyDef;


                    /** Definimos un cuerpo dinamico (afectado por la gravedad y resto de objetos box2d creados) */
                    //bodyDef.type = b2_dynamicBody;     /// la caja es dinamica
                    bodyDef.position.Set(posRaton.x * MPP, posRaton.y * MPP);


                    nuevaCaja.m_pBody = world.CreateBody(&bodyDef);  /// Creamos el body en el mundo y tenemos su puntero hacia el

                    polyShape.SetAsBox(50/2 * MPP, 20/2 * MPP); /// asignamos las dimensiones a la caja

                    fixtureDef.shape = &polyShape;
                    fixtureDef.friction = 0.2f;
                    fixtureDef.restitution	= 0.3f;
                    fixtureDef.density	= 0.7f;

                    nuevaCaja.m_pBody->CreateFixture(&fixtureDef);

                    m_vCajas.push_back(nuevaCaja);
                }

            }

            float anglePlayer = myPlayer.p_bodyPlayer->GetAngle();
            b2Vec2 posPlayer = myPlayer.p_bodyPlayer->GetPosition();


            switch(event.type) {
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::A) {
                        cout << " A Pressed!" << endl;

                        float impulse = myPlayer.p_bodyPlayer->GetMass() ;
                        myPlayer.p_bodyPlayer->ApplyLinearImpulse( b2Vec2(-impulse,0), myPlayer.p_bodyPlayer->GetWorldCenter(), true );
                    }
                    else if(event.key.code == sf::Keyboard::D){
                        cout << " D Pressed!" << endl;
                        float impulse = myPlayer.p_bodyPlayer->GetMass() ;
                        myPlayer.p_bodyPlayer->ApplyLinearImpulse( b2Vec2(impulse,0), myPlayer.p_bodyPlayer->GetWorldCenter(), true );
                    }
                    else if(event.key.code == sf::Keyboard::W){
                        cout << " W Pressed!" << endl;

                        float impulse = myPlayer.p_bodyPlayer->GetMass() * 5;
                        myPlayer.p_bodyPlayer->ApplyLinearImpulse( b2Vec2(0,-impulse), myPlayer.p_bodyPlayer->GetWorldCenter(), true );

                    }
                    else if(event.key.code == sf::Keyboard::Q) {
                        window.close();
                    }

                break;
        }

        }

        /// Limpiar ventana con el color negro
        window.clear(sf::Color::White);

        /// Actulizar mundo Box2D
        world.Step(TIMESTEP, VELITER, POSITER);

        /// Dibujamos las cajas creadas por el usuario
        for (std::vector<Caja>::iterator it = m_vCajas.begin(); it != m_vCajas.end(); ++it)
        {
            sf::RectangleShape rectCaja((*it).m_Size);
            rectCaja.setOrigin((*it).m_Size.x/2, (*it).m_Size.y/2);
            rectCaja.setFillColor(sf::Color::Cyan);

            float angle = (*it).m_pBody->GetAngle();
            b2Vec2 pos = (*it).m_pBody->GetPosition();

            rectCaja.setPosition(pos.x * PPM, pos.y * PPM);
            rectCaja.setRotation((angle * 180) / M_PI);

            window.draw(rectCaja); //Dibujamos el elemento en el buffer
        }

        /// Dibujando al player
        sf::RectangleShape playerCaja(myPlayer.sizePlayer);
        playerCaja.setOrigin(myPlayer.sizePlayer.x/2, myPlayer.sizePlayer.y/2);
        playerCaja.setFillColor(sf::Color::Black);

        float anglePlayer = myPlayer.p_bodyPlayer->GetAngle();
        b2Vec2 posPlayer = myPlayer.p_bodyPlayer->GetPosition();

        //  Ponemos el sprite
        sPlayer.setPosition(posPlayer.x * PPM,posPlayer.y * PPM);

        // Para ir actualizando la posicion
        playerCaja.setPosition(posPlayer.x * PPM, posPlayer.y * PPM);
        playerCaja.setRotation((anglePlayer * 180)/M_PI);
        window.draw(playerCaja);
        window.draw(sPlayer);


        /// Dibujamos un rectangulo que actuara de suelo mas adelante
        sf::RectangleShape rectSuelo(sf::Vector2f(800, 100));
        rectSuelo.setOrigin(800/2, 100/2);
        rectSuelo.setPosition(sf::Vector2f(sueloBody->GetPosition().x*PPM, sueloBody->GetPosition().y*PPM));
        rectSuelo.setFillColor(sf::Color::Cyan);
        window.draw(rectSuelo); //Dibujamos el elemento en el buffer

        /// Mostrar el buffer en pantalla
        window.display();
    }

    return 0;
}
