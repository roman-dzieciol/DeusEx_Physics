class PxActor extends Actor
	native;

var float oldZ;
var int NxActorPtr;

native function int SampleNativeFunction();

event SampleEvent(int i)
{
}

function PostBeginPlay()
{
	SampleNativeFunction();
}

function Timer()
{
}

simulated function Primitive GetPrimitive()
{
	if     ( Mesh != None   )	return LodMesh'DeusExDeco.CrateUnbreakableSmall';
	else if( Brush != None  )	return Brush;
	else              			return None; // Use Level->Engine->Cylinder
}

/*
singular function BaseChange()
{
    if( base == None )
        SetPhysics(PHYS_Falling);
}

function Bump( actor Other )
{
    local int augLevel, augMult;
    local float maxPush, velscale;
    local DeusExPlayer player;
    local Rotator rot;
	if( (Pawn(Other)!=None) && (Other.Mass > 40) )
	{
        // A little bit of a hack...
        // Make sure this decoration isn't being bumped from above or below
        if (abs(Location.Z-Other.Location.Z) < (CollisionHeight+Other.CollisionHeight-1))
        {
            maxPush = 100*2;
            augMult = 1*2;

            if (Mass <= maxPush)
            {
                // slow it down based on how heavy it is and what level my augmentation is
                velscale = FClamp((50.0 * augMult) / Mass, 0.0, 1.0);
                if (velscale < 0.25)
                    velscale = 0;

                    Velocity = Other.Velocity * velscale;

                if (Physics != PHYS_Falling)
                    Velocity.Z = 0;


                if (Physics != PHYS_Falling)
                    SetPhysics(PHYS_Rolling);

                Instigator = Pawn(Other);

                // Impart angular velocity (yaw only) based on where we are bumped from
                // NOTE: This is faked, but it looks cool
                rot = Rotator((Other.Location - Location) << Rotation);
                rot.Pitch = 0;
                rot.Roll = 0;

                // ignore which side we're pushing from
                if (rot.Yaw >= 24576)
                    rot.Yaw -= 32768;
                else if (rot.Yaw >= 8192)
                    rot.Yaw -= 16384;
                else if (rot.Yaw <= -24576)
                    rot.Yaw += 32768;
                else if (rot.Yaw <= -8192)
                    rot.Yaw += 16384;

                // scale it down based on mass and apply the new "rotational force"
                rot.Yaw *= velscale * 0.025;
                SetRotation(Rotation+rot);
            }
        }
	}
}*/

/*
	Mesh=LodMesh'DeusExDeco.CrateUnbreakableSmall'
	Mesh=LodMesh'DeusExCharacters.Cat'
	
	var(Collision) const bool bCollideActors;   // Collides with other actors.
var(Collision) bool       bCollideWorld;    // Collides with the world.
var(Collision) bool       bBlockActors;     // Blocks other nonplayer actors.
var(Collision) bool       bBlockPlayers;    // Blocks other player actors.
var(Collision) bool       bProjTarget;      // Projectiles should potentially target this actor.
*/     

defaultproperties
{ 
	bHidden=false
	DrawType=DT_Mesh
	Mesh=LodMesh'DeusExDeco.CrateUnbreakableSmall'
	CollisionRadius=16
	CollisionHeight=24
	Mass=40.000000
	Buoyancy=50.000000

	bCollideActors=True
	bCollideWorld=True
	bBlockActors=True
	bBlockPlayers=True
	bProjTarget=True


     bStatic=False
     Physics=PHYS_Falling
}
