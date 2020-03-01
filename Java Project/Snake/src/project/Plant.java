package project;

import java.awt.Color;
import java.util.List;

import game.Direction;
import naturesimulator.Action;
import naturesimulator.LocalInformation;
import ui.GridPanel;
/**
 * Plant is a child of Creature Class which can grow and reproduce.
 * Indirectly implements the Drawable Interface.
 * Main difference from Herbivore is, it cannot move and attack.
 * It's initial health value is 0.5, and its maximum health value is 1.0.
 * It's color is the shades of the color green.
 * 
 * @author cakmadam98
 *
 */
public class Plant extends Creature {
	/**
	 * Max_Health defines the maximum health a plant can have.
	 */
	private static final double MAX_HEALTH = 1.0;
	/**
	 * health field shows the current health of the plant.
	 */
	private double health;
	/**
	 * Constructs a Plant with given x-y coordinates and assign its health field to the given value.
	 * It uses super keyword to use the constructor of the Creature class.
	 * @param x x resembles x-coordinate of the plant.
	 * @param y y resembles y-coordinate of the plant.
	 */
	public Plant(int x, int y) {
		super(x,y);
		health = MAX_HEALTH/2.0;
	}
	
	/**
	 * returns the current health of the plant.
	 * @return the current health of the plant.
	 */
	public double getHealth() {
		return this.health;
	}

	/**
	 * sets the health of the plant to the given value.
	 * @param health new health of the creature.
	 */
	public void setHealth(double health) {
		this.health=health;
	}

	/**
	 * Plant chooses an action according to its priorities.
	 * Plant reproduces if there is available square nearby and its health equals 0.75 or bigger.
	 * If it cannot reproduce it should stay.
	 * If there is more than one available square to reproduce, it chooses one of them randomly.
	 * @param information representing the information a creature has about its surroundings.
	 * @return returns the convenient action.
	 */
	public Action chooseAction(LocalInformation information) {
		//Creates a list and stores the possible directions to go in there.
		List<Direction> possibleDirections = information.getFreeDirections();
		//From the list, it is chosen one direction randomly and store that direction in directionToGo.
		Direction directionToGo = LocalInformation.getRandomDirection(possibleDirections);
		//if statements show Plant's priorities.
		if(directionToGo!=null && this.health>=0.75) {
			return new Action(Action.Type.REPRODUCE, directionToGo);
		}
		else {
			return new Action(Action.Type.STAY);
		}
	}

	/**
	 * This method allows Plant to stay.
	 * During this period, it increases its health until it can reproduce.
	 */
	public void stay() {
		if(health<=0.95) {
			health += MAX_HEALTH/20.0;
		}
		else if(health>0.95) {
			health = MAX_HEALTH;
		}
	}
	/**
	 * This method is implemented due to abstract method of Creature Class.
	 * It is implemented empty, because of the reason that Plant cannot move.
	 * @param direction shows the direction to move.
	 */
	public void move(Direction direction) {
		
	}
	/**
	 * This method allows Herbivore to reproduce in the given direction.
	 * Creates a new plant in the given direction.
	 * Sets the new plant health to 10% of its parent’s health.
	 * The parent Plant’s health is reduced to 70% of its original health.
	 * @param direction new plant will be created in the given direction.
	 * @return returns the created creature.
	 */
	public Creature reproduce(Direction direction) {
		
		if(direction==Direction.DOWN) {
			Plant newPlant = new Plant(getX(),getY()+1);
			newPlant.setHealth(this.getHealth()/10.0); 
			this.setHealth(this.getHealth()*7.0/10.0); 
			return newPlant;
			
		} else if(direction==Direction.UP) {
			Plant newPlant = new Plant(getX(),getY()-1);
			newPlant.setHealth(this.getHealth()/10.0); 
			this.setHealth(this.getHealth()*7.0/10.0);
			return newPlant;
		} else if(direction==Direction.RIGHT) {
			Plant newPlant = new Plant(getX()+1,getY());
			newPlant.setHealth(this.getHealth()/10.0);
			this.setHealth(this.getHealth()*7.0/10.0); 
			return newPlant;
		} else if(direction==Direction.LEFT) {
			Plant newPlant = new Plant(getX()-1,getY());
			newPlant.setHealth(this.getHealth()/10.0); 
			this.setHealth(this.getHealth()*7.0/10.0); 
			return newPlant;
		}
		return null;
		
	}

	/**
	 * This method is implemented due to abstract method of Creature Class.
	 * It is implemented empty, because of the reason that Plant cannot attack.
	 * @param attackedCreature shows the target to attack(in this situation does not matter).
	 */
	public void attack(Creature attackedCreature) {
		
	}

	/**
	 * Prints the convenient color to the given pixel.
	 * Lighter the color, less health the Plant has.
	 * @param panel a grid panel that will be drawn on.
	 */
	public void draw(GridPanel panel) {
		if(this.health==MAX_HEALTH) {
			panel.drawSquare(getX(), getY(), new Color(30,150,10));
		} else if(this.health>=MAX_HEALTH/2.0) {
			panel.drawSquare(getX(), getY(), new Color(30,200,0));
		} else {
			panel.drawSquare(getX(), getY(), new Color(30,255,0));
		}
	}
	
}
