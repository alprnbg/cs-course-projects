package project;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

import game.Direction;
import naturesimulator.Action;
import naturesimulator.LocalInformation;
import ui.GridPanel;
/**
 * Herbivore moves around to eat Plants and reproduces when they accumulate enough health.
 * Herbivore class is a child of Creature class.
 * Indirectly implements Drawable interface.
 * Herbivore can reproduce, attack, move and stay.
 * Herbivore is represented with the shade of the color blue.
 * 
 * @author cakmadam98
 *
 */
public class Herbivore extends Creature {
	/**
	 * Max_Health defines the maximum health a herbivore can have.
	 */
	private static final double MAX_HEALTH = 20.0;
	/**
	 * health defines the current health of the herbivore.
	 */
	private double health;
	
	/**
	 * Constructs a Herbivore with given x-y coordinates and assign its health field to the given value.
	 * @param x resembles x-coordinate of the herbivore.
	 * @param y resembles y-coordinate of the herbivore.
	 */
	public Herbivore(int x, int y) {
		super(x,y);
		health=10.0;
	}
	/**
	 * returns the current health of the herbivore.
	 * @return the current health of the herbivore.
	 */
	public double getHealth() {
		return this.health;
	}
	/**
	 * sets the health of the herbivore to the given value.
	 * @param health new health of the creature.
	 */
	public void setHealth(double health) {
		this.health=health;
	}

	/**
	 * Herbivore chooses an action according to its priorities.
	 * Herbivore chooses to reproduce if its health is Max_Health.
	 * It it cannot reproduce, it will look for a Plant to attack.
	 * It it cannot attack, it will move to find a Plant. During this time, it will lose its health.
	 * If its health is 1.0 or below, it will stay.
	 * 
	 * @param information representing the information a creature has about its surroundings.
	 * @return returns the convenient action.
	 */
	public Action chooseAction(LocalInformation information) {
		/**
		 * Creates a list contains the free directions.
		 */
		List<Direction> possibleDirectionsToGo = information.getFreeDirections();
		/**
		 * Chooses a direction randomly from the list of free directions, by using static method of LocalInformation.
		 */
		Direction directionToGo = LocalInformation.getRandomDirection(possibleDirectionsToGo);
		/**
		 * Creates an ArrayList which will contain possible directions to attack.
		 */
		ArrayList<Direction> possibleDirectionsToAttack= new ArrayList<Direction>();
		
		/*
		 * This if statements looks for a Plant in particular directions and adds the direction to the ArrayList.
		 */
		if(information.getCreatureDown() instanceof Plant) {
			possibleDirectionsToAttack.add(Direction.DOWN);
		} if(information.getCreatureUp() instanceof Plant) {
			possibleDirectionsToAttack.add(Direction.UP);
		} if(information.getCreatureLeft() instanceof Plant) {
			possibleDirectionsToAttack.add(Direction.LEFT);
		} if(information.getCreatureRight() instanceof Plant) {
			possibleDirectionsToAttack.add(Direction.RIGHT);
		}
		
		//Chooses a direction randomly from possible directions to attack, by using static method of LocalInformation.
		Direction directionToAttack = LocalInformation.getRandomDirection(possibleDirectionsToAttack);
		
		
		//REPRODUCE if there is empty space and the health is maximum.
		if(directionToGo!=null && this.health==MAX_HEALTH) {
			return new Action(Action.Type.REPRODUCE, directionToGo);
		}
		//ATTACK if you cannot reproduce and there is a plant nearby.
		else if(directionToAttack!=null) {
			return new Action(Action.Type.ATTACK, directionToAttack);
		}
		//MOVE if you cannot attack and there is empty space.
		else if(directionToGo!=null && this.health>1.0) {
			return new Action(Action.Type.MOVE, directionToGo);
		}
		//STAY if your health is below 1.0.
		else {
			return new Action(Action.Type.STAY);
		}
	}
	/**
	 * This method allows Herbivore to stay.
	 * If herbivore's health is below 1.0, this method will be called.
	 * It will lose health during waiting period.
	 * If its health will be below zero, game logic will remove it. So I don't need to remove it by myself.
	 */
	public void stay() {
		this.health -= MAX_HEALTH/200.0;
	}
	
	/**
	 * This method allows Herbivore to move.
	 * If herbivore cannot attack a Plant, this method will called.
	 * It will lose health as it moves.
	 * This method cannot be called if Herbivore's health is below 1.0. (Which is implemented in chooseAction method.)
	 */
	public void move(Direction direction) {
		if(direction == Direction.UP) {
			setY(getY()-1);
		}
		else if(direction == Direction.DOWN) {
			setY(getY()+1);
		}
		else if(direction == Direction.LEFT) {
			setX(getX()-1);
		}
		else if(direction == Direction.RIGHT) {
			setX(getX()+1);
		}
		health -= MAX_HEALTH/20.0;
	}
	/**
	 * Priority of the Herbivore, is to reproduce. This method allows Herbivore to reproduce.
	 * It will reproduce if there is available space nearby and the health equals Max_Health.
	 * This method will create a new herbivore in the given direction. 
	 * Sets the newHerbivore's health to 20% of its parent’s health.
	 * The parent’s health is reduced to 40% of its original health
	 * @param direction new herbivore will be created in the given direction.
	 * @return returns the created herbivore.
	 */
	public Creature reproduce(Direction direction) {
		if(direction==Direction.DOWN) {
			Herbivore newHerbivore = new Herbivore(getX(),getY()+1);
			newHerbivore.setHealth(this.getHealth()/5.0);
			this.setHealth(this.getHealth()*4.0/10.0); 
			return newHerbivore;
			
		} if(direction==Direction.UP) {
			Herbivore newHerbivore = new Herbivore(getX(),getY()-1);
			newHerbivore.setHealth(this.getHealth()/5.0);
			this.setHealth(this.getHealth()*4.0/10.0);
			return newHerbivore;
			
		}if(direction==Direction.LEFT) {
			Herbivore newHerbivore = new Herbivore(getX()-1,getY());
			newHerbivore.setHealth(this.getHealth()/5.0);
			this.setHealth(this.getHealth()*4.0/10.0); 
			return newHerbivore;
			
		}if(direction==Direction.RIGHT) {
			Herbivore newHerbivore = new Herbivore(getX()+1,getY());
			newHerbivore.setHealth(this.getHealth()/5.0); 
			this.setHealth(this.getHealth()*4.0/10.0); 
			return newHerbivore;
		}
		return null;
	}

	/**
	 * Second priority of Herbivore is to attack. This method will allow Herbivore to attack.
	 * When a plant is eaten, its health will be added to Herbivore's health.
	 * Eaten plant's health will be assigned to 0 and game logic will remove the plant.
	 * Herbivore will move to the plant's location.
	 */
	public void attack(Creature attackedCreature) {
		//Adding Plant's health to the Herbivore's health.
		if(health+attackedCreature.getHealth()<=MAX_HEALTH) {
			health+=attackedCreature.getHealth();
		}
		else {
			health=MAX_HEALTH;
		}
		//Plant's health will be 0.
		attackedCreature.setHealth(0.0);
		//Herbivore will move to the plant's location.
		this.setX(attackedCreature.getX());
		this.setY(attackedCreature.getY());
	}

	/**
	 * Prints the convenient color to the given pixel.
	 * Lighter the color, less health the Herbivore has.
	 * @param panel a grid panel that will be drawn on.
	 */
	public void draw(GridPanel panel) {
		if(this.health==MAX_HEALTH) {
			panel.drawSquare(getX(), getY(), new Color(0,50,150));
		} else if(this.health>=MAX_HEALTH/20.0){
			panel.drawSquare(getX(), getY(), new Color(0,50,200));
		} else {
			panel.drawSquare(getX(), getY(), new Color(0,50,250));
		}
	}
}