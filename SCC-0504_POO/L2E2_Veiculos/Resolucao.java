
import java.util.Scanner;



/**
 * Classe principal do programa.
 * 
 * @author Matheus Carvalho Raimundo
 */
class Main {

	/**
	 * Chamada principal do programa.
	 * 
	 * @param args argumentos de linha de comando
	 */
	public static void main(String[] args){

	Vehicle[] vehicles;
	Scanner fStream;
	int i, N;

	fStream = new Scanner(System.in);
	vehicles = new Vehicle[] {
		new Car(),
		new Motorcycle(),
		new Bike()
	};

	Vehicle.accelerationBase = fStream.nextDouble();
	N = fStream.nextInt();

	for(Vehicle vehicleAux : vehicles){
		for(i = 0; i < N; i++){
			vehicleAux.turnOn();
			switch(fStream.next()){
				case "A":
					vehicleAux.accelerate(fStream.nextDouble());
				break;
				case "F":
					vehicleAux.brake(fStream.nextDouble());
				break;
				default:
					vehicleAux.move(fStream.nextDouble());
			}
		}
	}

	for(Vehicle vehicleAux : vehicles){
		vehicleAux.turnOff();
		System.out.print(vehicleAux.getDistance() + " ");
	}
	System.out.println();

	fStream.close();

	}

}



/**
 * Classe que representa um veículo genérico.
 * 
 * @author Matheus Carvalho Raimundo
 */
class Vehicle {

	/**
	 * Variável que indica a aceleração base.
	 * É uma variável pública porque, por escolha de projeto,
	 *   qualquer um pode alterar a aceleração base por livre
	 *   e espontânea vontade a qualquer momento, e isso não
	 *   afetaria negativamente as instâncias da classe.
	 * 
	 */
	public static double accelerationBase;

	/**
	 * Variáveis de controle do veículo.
	 * 
	 */
	protected double
		velocity,
		distance,
		accelerationRatio;

	/**
	 * Indica se o veículo está ou não ligado/em uso.
	 * 
	 */
	protected boolean on;

	/**
	 * Construtor padrão para um veículo genérico.
	 * 
	 */
	public Vehicle(){
		this(1);
	}

	/**
	 * Construtor para um veículo genérico.
	 * 
	 * @param accelerationBase aceleração base
	 */
	public Vehicle(double accelerationRatio){
		this.velocity = 0;
		this.distance = 0;
		this.accelerationRatio = accelerationRatio;
		this.on = false;
	}

	/**
	 * Liga o veículo.
	 * 
	 */
	public void turnOn(){
		this.on = true;
	}

	/**
	 * Desliga o veículo.
	 * 
	 */
	public void turnOff(){
		this.on = false;
	}

	/**
	 * O veículo move com velocidade constante (sem aplicar aceleração).
	 * 
	 * @param t o tempo na qual o veículo será movimentado
	 * @return true se for movimentado com sucesso, false caso contrário
	 */
	public boolean move(double t){
		if(t <= 0)  return false;
		this.distance += this.velocity * t;
		return true;
	}

	/**
	 * Acelera o veículo com a aceleração normal dele.
	 * 
	 * @param t o tempo na qual o veículo será acelerado
	 * @return true se for acelerado com sucesso, false caso contrário
	 */
	public boolean accelerate(double t){
		if(t <= 0 || !this.on)   return false;
		double a = this.accelerationRatio * accelerationBase;
		this.distance += (this.velocity * t) + (a * t * t) / 2;
		this.velocity += a * t;
		return true;
	}

	/**
	 * Aplica os freios no veículo com a aceleração oposta da aceleração normal dele.
	 * 
	 * @param t o tempo na qual os freios serão aplicados
	 * @return true se for aplicado os freios com sucesso, false caso contrário
	 */
	public boolean brake(double t){
		if(t <= 0 || !this.on)   return false;
		double a = this.accelerationRatio * accelerationBase;
		this.distance += (this.velocity * t) - (a * t * t) / 2;
		this.velocity -= a * t;
		return true;
	}

	/**
	 * Obtém a distância percorrida até o dado momento.
	 * 
	 * @return a distância
	 */
	public double getDistance(){
		return this.distance;
	}

}


/**
 * Classe que representa uma bicicleta.
 * 
 * @author Matheus Carvalho Raimundo
 */
class Bike extends Vehicle {

	/**
	 * Construtor padrão para a classe, que chama o construtor da classe pai
	 *   passando a razão de aceleração da bicicleta.
	 * 
	 */
	public Bike(){
		super(1);
	}

}


/**
 * Classe que representa um carro.
 * 
 * @author Matheus Carvalho Raimundo
 */
class Car extends Vehicle {

	/**
	 * Construtor padrão para a classe, que chama o construtor da classe pai
	 *   passando a razão de aceleração do carro.
	 * 
	 */
	public Car(){
		super(3);
	}

}


/**
 * Classe que representa uma motocicleta.
 * 
 * @author Matheus Carvalho Raimundo
 */
class Motorcycle extends Vehicle {

	/**
	 * Construtor padrão para a classe, que chama o construtor da classe pai
	 *   passando a razão de aceleração da motocicleta.
	 * 
	 */
	public Motorcycle(){
		super(4.5);
	}

}


